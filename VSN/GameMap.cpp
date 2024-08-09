#include "pch.h"
#include "GameMap.h"

GameMap::GameMap(GameInfoData& data)
	: _gameId(data.GameId),
	_spawnableItems(ITEM_COUNTS, false),
	_prep(data)
{
	// Create Players
	for (auto& p : data.Players)
	{
		auto player = NetCore::make_shared<Player>(++_nidProducer, p);

		_players.insert({ ++_nidProducer, player });
		_playerIdToNid.insert({ p.AccountDbId, player->Nid()});
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

	ASSERT_CRASH(_mapData->initial_player_spawnpoint()->size() >= GameConst::MAX_PLAYER_COUNT);

	// init spawn point of players
	int i = 0;
	for (const auto& p : _players)
	{
		auto pos = _mapData->initial_player_spawnpoint()->Get(i);
		p.second->_position = Vector2(pos->x(), pos->y());
		i++;
	}

	// other game objects start at nid=101
	_nidProducer = 100;
}

GameMap::~GameMap()
{
}


bool GameMap::TryPlayerConnect(const std::string& token, std::shared_ptr<GameSession> session)
{
	const uint playerId = session->GetAccountDbId();
	// success
	if (_prep.PlayerConnected(playerId, token))
	{
		// error
		ASSERT_CRASH(_playerIdToNid.find(playerId) != _playerIdToNid.end());

		const auto nid = _playerIdToNid.at(playerId);
		return true;
	}
	// fail
	else
	{
		return false;
	}
}

void GameMap::PlayerReady(const uint accountDbId)
{
	_prep.PlayerReady(accountDbId);
	if (_prep.AllPlayerReadyToStart())
	{
		// all players are ready to start
		StartCountDown(10);
	}	
}

void GameMap::StartCountDown(int count)
{
	if (count == 0)
	{
		// Start game
		Broadcast(Packets::Simple(START_GAME, "Start Game"));
	}
	else
	{
		// Send count down pkt to clients
		Broadcast(Packets::CountDown(count));
		count--;
		DISCARD PushReservableJob(1000, &GameMap::StartCountDown, count);
	}
}

void GameMap::Broadcast(Packet&& pkt)
{
	for (auto& p : _players)
	{
		p.second->Session().lock()->Send(pkt.id, pkt.Buf(), pkt.size);
	}
}
