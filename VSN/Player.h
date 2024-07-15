#pragma once

class GameSession;

class Player : public GameObject
{
public:
	Player(const uint nid, const PlayerData data);
	~Player();

	void Update(VSN::PlayerState state);

	void SetSession(weak_ptr<GameSession> session) { _session = session; }
	void Ready() { _ready = true; }
	bool IsReady() const { return _ready; }
	PlayerData& GetPlayerData() { return _data; }
	std::weak_ptr<GameSession> Session() const { return _session; }
private:
	uint _nid;
	bool _ready;
	std::weak_ptr<GameSession> _session;
	PlayerData _data;
	int _hp;
};

