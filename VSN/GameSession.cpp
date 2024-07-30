#include "pch.h"
#include "GameSession.h"

GameSession::GameSession() 
	: _remoteIp(""), _port(UNSIGNED_INVALID),
	_accountDbId(UNSIGNED_INVALID), _ready(false)
{
}

GameSession::~GameSession()
{
}

void GameSession::TryConnectToMap(const VSN::ReqGameInfo* info)
{
	const auto map = GGameManager->Map(info->game_id());
	if (map == nullptr)
	{
		const auto res = Packets::ResGameInfo(false, {});
		Send(res.id, res.Buf(), res.size);

		DisconnectByMap("Invalid Game Id");
		return;
	}

	if(map->TryPlayerConnect(info->auth_token()->str(), SharedFromThis()) == false)
	{
		const auto res = Packets::ResGameInfo(false, {});
		Send(res.id, res.Buf(), res.size);

		DisconnectByMap("Invalid Player Data");
		return;
	}

	_accountDbId = info->account_id();

	// send to game info to client
	auto pos = map->GetPosition(_accountDbId);
	const auto res = Packets::ResGameInfo(
		true,
		{},
		map->GameId(),
		map->Difficulty(),
		{ pos.first, pos.second }
	);
	Send(res.id, res.Buf(), res.size);


	LOG(INFO) << "Player [" << _accountDbId << "] joined game: " << info->game_id() << '\n';
}

void GameSession::OnConnected()
{
	// TODO : use constant size(about sockaddr) instead of 50.
	wchar_t wip[50]{};
	if (Session::GetPeerName(&_port, wip, sizeof(wip)) == false)
	{
		int error = ::WSAGetLastError();
		LOG(INFO) << "Failed to get remote peer name at id=" << Session::SessionId();
		LOG(INFO) << "Error: " << error;
		Session::Disconnect();
		return;
	}

	_remoteIp = NetCore::Utils::ToString(wip);

	LOG(INFO) << "Connected to client " << _remoteIp << ':' << _port;

	GSessionManager->AddConnectedSession(SharedFromThis());
}

void GameSession::OnSend(const int len)
{
	cout << "Sent: " << len << " bytes to " << _remoteIp << '\n';
}

void GameSession::OnDisconnected(const int error)
{
	cout << "Disconnected from " << _remoteIp << " by " << error << '\n';
}

void GameSession::OnRecvPacket(const char* buffer, const ushort id)
{
	GPacketManager->Dispatch(id, buffer, SharedFromThis());
}
