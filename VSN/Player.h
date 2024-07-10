#pragma once

class GameSession;

class Player : public GameObject
{
public:
	Player(weak_ptr<GameSession> session, const uint nid, const uint characterTypeId);
	~Player();

	void Update(VSN::PlayerState state);

	std::weak_ptr<GameSession> Session() const { return _session; }
private:
	std::weak_ptr<GameSession> _session;

	int _hp;
};

