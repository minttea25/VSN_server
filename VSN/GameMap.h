#pragma once

class Player;
class Monster;

//class GameMapData
//{
//public:
//	GameMapData(const VSN::MapData* data)
//	{
//	}
//	~GameMapData() {}
//private:
//	uint _mapType;
//	Vector2 _mapSize;
//	NetCore::Vector<Vector2> _initialPlayerSpawnPoint;
//};

class GameMap : public NetCore::GlobalTimeJobSerializer
{
	// TEMP
	static const uint ITEM_COUNTS = 5;
public:
	GameMap(GameInfoData data);
	~GameMap();

	bool PlayerConnected(std::shared_ptr<GameSession> session);
	void PlayerReady(const uint accountDbId);

	void StartGame();

	void UpdatePlayer(const uint nid, VSN::PlayerState state)
	{
		// TODO : copy state
		//ReserveJob(0, &GameMap::_updatePlayer, nid, state);
	}

	void Broadcast(Packet&& pkt);

	uint Id() const { return _gameId; }

private:

private:
	const uint _gameId;
	const VSN::MapData* _mapData;

	// [nid, player]
	NetCore::HashMap<uint, std::shared_ptr<Player>> _players;
	// [nid, monster]
	NetCore::HashMap<uint, std::shared_ptr<Monster>> _monsters;

	// [accountDbId, player]
	NetCore::HashMap<uint, std::shared_ptr<Player>> _dbIdToPlayer;
	
	NetCore::Vector<bool> _spawnableItems;
	ushort _difficulty;
	uint _mapTypeId;

	uint _nidProducer = 0;

};

