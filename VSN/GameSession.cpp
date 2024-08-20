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

void GameSession::TryConnectToMap(const VSN::ConnectGame* data)
{
	_accountDbId = data->player_uid();

	const auto map = GGameManager->Map(data->game_id());
	if (map == nullptr)
	{
		const auto res = Packets::GameLoadInfo(false, _accountDbId);
		Send(res.id, res.Buf(), res.size);

		DisconnectByMap("Invalid Game Id");
		return;
	}

	uint nid = map->TryPlayerConnect(data->auth_token()->str(), SharedFromThis());
	if (nid == 0)
	{
		const auto res = Packets::GameLoadInfo(false, _accountDbId);
		Send(res.id, res.Buf(), res.size);

		DisconnectByMap("Invalid Player Data");
		return;
	}

	// send to game info to client
	// TEMP
	const auto res = Packets::GameLoadInfo(
		true,
		/*player nid*/nid,
		/*spawnable items*/{1, 2, 3, 4, 5},
		map->GameId(),
		map->Difficulty(),
		/*TODO: player spawns*/ map->PlayerSpawnInfo()
	);
	Send(res.id, res.Buf(), res.size);


	LOG(INFO) << "Player [" << _accountDbId << "] joined game: " << data->game_id()  << " with NID=" << nid << '\n';
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
