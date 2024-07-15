#pragma once
class GameSession : public NetCore::PacketSession
{
public:
	GameSession();
	~GameSession();

	void SetInfo(const uint gameId, const uint accountDbId, const std::string& authToken);

	/*inline-methods*/
	std::shared_ptr<GameSession> SharedFromThis()
	{
		return static_pointer_cast<GameSession>(shared_from_this());
	}

	void SetPlayer(std::shared_ptr<Player> player)
	{
		_player = player;
	}
	std::string GetAuthToken() const { return _authToken; }
	std::shared_ptr<class Player> GetPlayer() const { return _player; }
	uint GetAccountDbId() const { return _accountDbId; }

	/*overrides from PacketSession*/
	void OnConnected() override;
	void OnSend(const int len) override;
	void OnDisconnected(const int error) override;
	void OnRecvPacket(const char* buffer, const ushort id) override;


private:
	std::string _remoteIp;
	ushort _port;
	
	bool _ready;
	uint _accountDbId;
	std::string _authToken;
	std::shared_ptr<class Player> _player;

};

