#include "pch.h"
#include "GameServer.h"

GameServer::GameServer(const std::wstring& ip, const ushort port, const ushort sessionCount, const ushort backlog)
	: _ip(ip), _port(port)
{
	// Initialize global classes
	GPacketManager = NetCore::make_shared<PacketManager>();
	GSessionManager = NetCore::make_shared<SessionManager>();
	GGameManager = NetCore::make_shared<GameManager>();

	_session_factory = std::bind(&SessionManager::SessionFactory, GSessionManager);

	_addr = NetCore::AddrUtils::GetTcpAddress(ip, port);
	_core = NetCore::make_shared<NetCore::IOCPCore>();;
	_service = NetCore::make_shared<NetCore::ServerService>
		(
			_core, _addr, _session_factory, sessionCount, backlog
		);
	_manager = NetCore::make_shared<NetCore::TaskManagerEx>();
}

GameServer::~GameServer()
{
	_manager->JoinAllTasks();

	_core = nullptr;
	_service = nullptr;
	_manager = nullptr;
}

bool GameServer::Start()
{
	if (!_service->IsStarted() && _service->Start() == false)
	{
		LOG(ERROR) << "Failed to start server service.";

		return false;
	}
	else
	{
		LOG(INFO) << "Starting server on " << NetCore::Utils::ToString(_ip) << ":" << _port;
		return true;
	}
}

bool GameServer::Stop()
{
	LOG(INFO) << "Stopping the server service...";

	_off = true;

	if (!_service->Stop())
	{
		LOG(ERROR) << "Failed to stop server service.";
		return false;
	}
	else return true;
}

void GameServer::StartWork(const int threadCount)
{
	_manager->AddTask([&]() 
		{
			_netCoreWork();
		}, threadCount);
}

void GameServer::_netCoreWork()
{
	LOG(INFO) << "Working IOCP thread at tid=" << NetCore::TLS_Id;

	while (true)
	{
		if (_off) break;

		_core->ProcessQueuedCompletionStatusEx(GET_STATUS_TIMEOUT_MILLISECONDS);

		_manager->DoWorkJob();

		_manager->DoWorkReservedJob(RESERVED_JOB_DURATION);

		GSessionManager->FlushSessions();

		this_thread::yield();
	}
}

