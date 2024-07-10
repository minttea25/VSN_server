#include "pch.h"
#include "Server.h"

void WebSession::OnConnected()
{
	wchar_t wip[50]{};
	if (Session::GetPeerName(&_port, wip, sizeof(wip)) == false)
	{
		int error = ::WSAGetLastError();
		LOG(INFO) << "Failed to get peer name of websession: " << error;
		Session::Disconnect();
		return;
	}

	_remoteIp = NetCore::Utils::ToString(wip);

	LOG(INFO) << "WebSession is connected to " << _remoteIp << ':' << _port;
}

void WebSession::OnSend(const int len)
{
}

void WebSession::OnDisconnected(const int error)
{
	LOG(INFO) << "WebSession is disconnected by " << error;
}

void WebSession::OnRecvPacket(const char* buffer, const ushort id)
{
	// TODO
}

Server::Server(const std::wstring& ip, const ushort port)
	: _ip(ip), _port(port)
{
	std::function<std::shared_ptr<NetCore::Session>()> factory
		= []() -> std::shared_ptr<NetCore::Session> {
		return NetCore::make_shared<WebSession>();
		};

	_addr = NetCore::AddrUtils::GetTcpAddress(ip, port);
	_core = NetCore::make_shared<NetCore::IOCPCore>();;
	_service = NetCore::make_shared<NetCore::ServerService>
		(
			_core, _addr, factory, 1, 5
		);
	_manager = NetCore::make_shared<NetCore::TaskManagerEx>();
}

Server::~Server()
{
	_manager->JoinAllTasks();

	_core = nullptr;
	_service = nullptr;
	_manager = nullptr;
}

bool Server::Start()
{
	if (!_service->IsStarted() && _service->Start() == false)
	{
		LOG(ERROR) << "Failed to start service for web session.";
		return false;
	}
	else
	{
		LOG(INFO) << "Waiting the connection of web session on " << NetCore::Utils::ToString(_ip) << ":" << _port;
		
		_manager->AddTask([&]() {
			LOG(INFO) << "[Web Session Server] Working IOCP thread";

			while (true)
			{
				if (_off) break;

				_core->ProcessQueuedCompletionStatusEx(GET_STATUS_TIMEOUT_MILLISECONDS);

				// No data to send => no flush about session

				this_thread::yield();
			}
			});
		
		return true;
	}
}

bool Server::Stop()
{
	LOG(INFO) << "Stopping the service for web session...";

	_off = true;

	if (!_service->Stop())
	{
		LOG(ERROR) << "Failed to stop the service for web session.";
		return false;
	}
	else return true;
}
