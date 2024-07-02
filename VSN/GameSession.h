#pragma once
class GameSession : public NetCore::PacketSession
{
public:
	GameSession();
	~GameSession();

	/*inline-methods*/
	std::shared_ptr<GameSession> SharedFromThis()
	{
		return static_pointer_cast<GameSession>(shared_from_this());
	}

	std::shared_ptr<class Player> GetPlayer() const { return _player; }

	/*overrides from PacketSession*/
	void OnConnected() override;
	void OnSend(const int len) override;
	void OnDisconnected(const int error) override;
	void OnRecvPacket(const char* buffer, const ushort id) override;


private:
	std::string _remoteIp;
	ushort _port;

	std::shared_ptr<class Player> _player;

};

