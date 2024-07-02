#pragma once

class GameSession;

class Player : public enable_shared_from_this<Player>
{
public:
	uint InMapId;

	Player(uint id, std::weak_ptr<GameSession> session);

	uint Id() const { return _id; }
	std::weak_ptr<GameSession> Session() const { return _session; }
private:
	uint _id;
	std::weak_ptr<GameSession> _session;
};

