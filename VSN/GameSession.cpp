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

void GameSession::SetInfo(const uint gameId, const uint accountDbId, const std::string& authToken)
{
	_authToken = authToken;
	_accountDbId = accountDbId;

	auto map = GGameManager->Map(gameId);
	if (map == nullptr) Disconnect();
	if (map->PlayerConnected(SharedFromThis()) == false) Disconnect();
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
