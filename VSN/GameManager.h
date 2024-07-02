#pragma once
class GameManager
{
public:
	GameManager();
	~GameManager();

	std::shared_ptr<class GameMap> Map(const uint mapId)
	{
		return _maps[mapId];
	}

	void CreateNewMap();

	void Broadcast(Packet& pkt);

private:
	NetCore::_USE_LOCK;
	NetCore::HashMap<uint, std::shared_ptr<class GameMap>> _maps;
};

