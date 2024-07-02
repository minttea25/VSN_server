#include "pch.h"
#include "GameManager.h"

GameManager::GameManager()
{
	// TEMP
	{
		auto map = NetCore::ObjectPool<GameMap>::make_shared();
		map->Init();

		_maps.insert({ map->Id(), map });
	}
}

GameManager::~GameManager()
{
}

void GameManager::CreateNewMap()
{
	// TEMP : It should not be called now.
	NetCore::_WRITE_LOCK;

	auto map = NetCore::ObjectPool<GameMap>::make_shared();
	map->Init();

	_maps.insert({ map->Id(), map });
}

void GameManager::Broadcast(Packet& pkt)
{
	for (auto& s : GSessionManager->_sessions)
	{
		s.second->Send(pkt.id, pkt.Buf(), pkt.size);
	}
}
