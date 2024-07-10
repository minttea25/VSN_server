#pragma once
class GameLobby : public NetCore:: GlobalJobSerializer
{
public:
	GameLobby();
	~GameLobby();

	void Init();

	void PlayerJoin(std::shared_ptr<Player> player, const uint req)
	{
		PushJob(&GameLobby::_playerJoin, player, req);
	}

	void PlayerReady(std::shared_ptr<Player> player, const uint req)
	{
		PushJob(&GameLobby::_playerReady, player, req);
	}

	bool CheckAllPlayerReady() const
	{
		return 0b1111u == _readySet;
	}

	void SetReady(const uint inMapId)
	{
		_readySet |= (1 << (inMapId - 1));
	}

	void StartGame();

	void UpdatePlayer(const uint nid, VSN::PlayerState& state)
	{
		ReserveJob(0, &GameLobby::_updatePlayer, nid, state);
	}

	uint Id() const { return _gameId; }

private:
	void _playerJoin(std::shared_ptr<Player> player, const uint req);
	void _playerReady(std::shared_ptr<Player> player, const uint req);
	void _startLoadGame();

private:
	static constexpr uint START_COUNTDOWN = 3000;
	static constexpr uint MAX_PLAYER = 4;
	static constexpr uint MAX_PLAYER_SET = 0xFFFF;

	uint _gameId;
	std::shared_ptr<Player> _players[MAX_PLAYER]{ nullptr, nullptr, nullptr, nullptr };
	NetCore::_byte _readySet;
};

