#pragma once

class Player;
class Monster;

class GamePreparation
{
public:
	GamePreparation(const GameInfoData& data)
	{
		ASSERT_CRASH(data.Players.size() == GameConst::MAX_PLAYER_COUNT);

		// mapping id to idx
		for (int i = 0; i < GameConst::MAX_PLAYER_COUNT; ++i)
		{
			const auto& p = data.Players.at(i);
			_idToIdx.insert({ p.AccountDbId, i });
			_playerAuthTokens[i] = p.PlayerAuthToken;
			_playerReady[i] = false;
		}
	}

	bool PlayerConnected(const uint playerId, const std::string& token)
	{
		int i = _get(playerId);
		if (0 == strcmp(token.c_str(), _playerAuthTokens[i].c_str()))
		{
			// authed and connected
			_playerConnected[i] = true;
			return true;
		}
		else return false;
	}

	void PlayerReady(const uint playerId)
	{
		_playerReady[_get(playerId)] = true;
	}

	bool AllPlayerReadyToStart()
	{
		for (const auto& status : _playerReady)
		{
			if (status == false) return false;
		}
		return true;
	}

private:
	int _get(uint playerId) const { return _idToIdx.at(playerId); }
private:
	NetCore::HashMap<uint, int> _idToIdx;
	std::string _playerAuthTokens[GameConst::MAX_PLAYER_COUNT];
	bool _playerConnected[GameConst::MAX_PLAYER_COUNT] = { false, };
	bool _playerReady[GameConst::MAX_PLAYER_COUNT] = { false, };

};

class GameMap : public NetCore::JobSerializer
{
	// TEMP
	static const uint ITEM_COUNTS = 5;
public:
	GameMap(GameInfoData& data);
	~GameMap();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="token"></param>
	/// <param name="session"></param>
	/// <returns>Nid of player</returns>
	uint TryPlayerConnect(const std::string& token, std::shared_ptr<GameSession> session);
	void PlayerReady(const uint accountDbId);

	void StartCountDown(int count);

	void UpdatePlayer(const uint nid, VSN::PlayerState state)
	{
		// TODO : copy state
	}

	void Broadcast(Packet&& pkt);

	uint GameId() const { return _gameId; }
	unsigned short Difficulty() const { return _difficulty; }

	NetCore::Vector<VSN::PlayerSpawnInfo> PlayerSpawnInfo()
	{
		NetCore::Vector<VSN::PlayerSpawnInfo> v;
		for (auto& p : _players)
		{
			auto pos = p.second->Position();
			v.push_back(VSN::PlayerSpawnInfo(p.first, VSN::Vector2(pos.x, pos.y)));
		}
		return v;
	}

	std::shared_ptr<GameMap> Shared_from_this()
	{
		return static_pointer_cast<GameMap>(shared_from_this());
	}

private:
	const uint _gameId;
	const VSN::MapData* _mapData;
	GamePreparation _prep;
	NetCore::Atomic<int> _connectedCount = 0;

	// [nid, player]
	NetCore::HashMap<uint, std::shared_ptr<Player>> _players;
	// [nid, monster]
	NetCore::HashMap<uint, std::shared_ptr<Monster>> _monsters;

	// [playerId, nid]
	NetCore::HashMap<uint, uint> _playerIdToNid;

	NetCore::Vector<bool> _spawnableItems;
	ushort _difficulty;
	uint _mapTypeId;

	uint _nidProducer = 0;
};
