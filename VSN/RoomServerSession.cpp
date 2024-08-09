#include "pch.h"
#include "RoomServerSession.h"

void RoomServerSession::OnConnected()
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

	LOG(INFO) << "RoomServerSession is connected to " << _remoteIp << ':' << _port;
}

void RoomServerSession::OnSend(const NetCore::int32 len)
{
}

NetCore::uint32 RoomServerSession::OnRecv(const NetCore::_byte* buffer, const NetCore::uint32 len)
{
	const auto pkt = VSN::GetWebGameInfoData(buffer);

	if (pkt == nullptr)
	{
		std::cerr << "Wrong buffer to parse GameInfoData.";
		return 0;
	}

	DebugUtil::Show(pkt);

	const int pSize = pkt->player_account_id()->size();
	std::vector<PlayerData> pData;
	for (int i = 0; i < pSize; ++i)
	{
		pData.push_back(
			PlayerData
			(
				pkt->player_account_id()->Get(i),
				pkt->player_character_type()->Get(i),
				pkt->player_auth_token()->Get(i)->str()
			)
		);
	}

	std::vector<uint> items;
	for (int i = 0; i < pkt->spawnable_items()->size(); ++i)
	{
		items.push_back(pkt->spawnable_items()->Get(i));
	}
	MapConfiguration mapConfig(pkt->map_type_id(), items, pkt->difficulty());

	GameInfoData data(pkt->game_id(), pData, mapConfig);
	GGameManager->CreateNewMap(data);

	_managedMaps.insert({ pkt->game_id(), GGameManager->Map(pkt->game_id()) });

	// TODO : disconnect
	Disconnect();

	return len;
}

void RoomServerSession::OnDisconnected(const NetCore::int32 error)
{
	LOG(INFO) << "WebSession is disconnected by " << error;
}

Server::Server(const std::wstring& ip, const ushort port)
	: _ip(ip), _port(port)
{
	std::function<std::shared_ptr<NetCore::Session>()> factory
		= []() -> std::shared_ptr<NetCore::Session> {
		return NetCore::make_shared<RoomServerSession>();
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
			LOG(INFO) << "[Room Session Server] Working IOCP thread";

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

PURE_VIRTUAL void RoomServerSession::SendRaw(const NetCore::_byte* buffer)
{
	return PURE_VIRTUAL void();
}

PURE_VIRTUAL void RoomServerSession::Send(const NetCore::uint16 id, NetCore::_ubyte* ptr, const NetCore::uint16 size)
{
	return PURE_VIRTUAL void();
}
