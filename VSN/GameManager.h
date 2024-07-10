#pragma once
class GameManager
{
public:
	GameManager();
	~GameManager();

	std::shared_ptr<class GameMap> Map(const uint gameId)
	{
		return _maps[gameId];
	}

	void CreateNewMap(GameInfoData data);

	void DestroyMap(const uint gameId);

	void Broadcast(Packet& pkt);

private:
	NetCore::_USE_LOCK;
	NetCore::HashMap<uint, std::shared_ptr<class GameMap>> _maps;
};

