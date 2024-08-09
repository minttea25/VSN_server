#include "pch.h"
#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::CreateNewMap(GameInfoData data)
{
	auto map = NetCore::make_shared<GameMap>(data);

	NetCore::_WRITE_LOCK;
	_maps.insert({ data.GameId, map });
}

void GameManager::DestroyMap(const uint gameId)
{
	NetCore::_WRITE_LOCK;
	auto s = _maps.erase(gameId);
	if (s == 1) { /*ok*/ }
	else { /*error*/ }
}

void GameManager::Broadcast(Packet& pkt)
{
	for (auto& s : GSessionManager->_sessions)
	{
		s->Send(pkt.id, pkt.Buf(), pkt.size);
	}
}
