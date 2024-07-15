#include "pch.h"
#include "GameMap.h"

GameMap::GameMap(GameInfoData data)
	: _gameId(data.GameId),
	_spawnableItems(ITEM_COUNTS, false)
{
	// Create Players
	for (auto& p : data.Players)
	{
		auto player = NetCore::make_shared<Player>(++_nidProducer, p);

		_players.insert({ ++_nidProducer, player });
		_dbIdToPlayer.insert({ p.AccountDbId, player });
	}

	// Set Items spawnable
	for (auto& itemId : data.MapConfig.SpawnableItems)
	{
		if (itemId < ITEM_COUNTS)
		{
			_spawnableItems[itemId] = true;
		}
	}

	// Set difficulty
	_difficulty = data.MapConfig.Difficulty;

	// Set mapTypeId
	_mapTypeId = data.MapConfig.MapTypeId;

	std::vector<uint8_t> buf;
	GDataManager->LoadMapData(_mapTypeId, OUT buf);
	_mapData = VSN::GetMapData(buf.data());

	DebugUtil::Show(_mapData);
}

GameMap::~GameMap()
{
}


bool GameMap::PlayerConnected(std::shared_ptr<GameSession> session)
{
	// It means the player is connected to server.

	// check
	const uint id = session->GetAccountDbId();
	if (_dbIdToPlayer.find(id) == _dbIdToPlayer.end())
	{
		return false;
	}
	const PlayerData& data = _dbIdToPlayer[id]->GetPlayerData();
	if (std::strcmp(data.PlayerAuthToken.c_str(), session->GetAuthToken().c_str()) != 0)
	{
		return false;
	}

	session->SetPlayer(_dbIdToPlayer[id]);
	_dbIdToPlayer[id]->SetSession(session);

	// send info to client


	return true;
}

void GameMap::PlayerReady(const uint accountDbId)
{
	if (_dbIdToPlayer.find(accountDbId) != _dbIdToPlayer.end())
	{
		_dbIdToPlayer[accountDbId]->Ready();
	}

	for (auto& p : _dbIdToPlayer)
	{
		if (p.second->IsReady() == false) return;
	}

	// all players are ready to start
	StartGame();
}

void GameMap::StartGame()
{
	NetCore::GGlobalJobWorker->AddTimeJobQueue(shared_from_this());

	// TEMP
	Broadcast(Packets::Simple(SimpleId::SP_GAME_START, ""));
}

void GameMap::Broadcast(Packet&& pkt)
{
	for (auto& p : _dbIdToPlayer)
	{
		if (p.second->IsReady())
		{
			p.second->Session().lock()->Send(pkt.id, pkt.Buf(), pkt.size);
		}
	}
}
