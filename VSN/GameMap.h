#pragma once

class Player;
class Monster;

class GameMap : public NetCore::GlobalTimeJobSerializer
{
public:
	GameMap(GameInfoData data);
	~GameMap();

	void Init();

	void StartGame();

	void UpdatePlayer(const uint nid, VSN::PlayerState state)
	{
		// TODO : copy state
		//ReserveJob(0, &GameMap::_updatePlayer, nid, state);
	}

	uint Id() const { return _gameId; }

private:

private:
	const uint _gameId;
	NetCore::HashMap<uint, std::shared_ptr<Player>> _players;
	NetCore::HashMap<uint, std::shared_ptr<Monster>> _monsters;


};

