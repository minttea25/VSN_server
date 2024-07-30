#pragma once

class GameSession;

class Player : public GameObject
{
public:
	Player(const uint nid, const PlayerData data);
	~Player();

	void Update(VSN::PlayerState state);

	void SetSession(weak_ptr<GameSession> session) { _session = session; }
	std::weak_ptr<GameSession> Session() const { return _session; }
private:
	uint _nid;
	std::weak_ptr<GameSession> _session;
	int _hp;
};

