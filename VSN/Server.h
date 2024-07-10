#pragma once

class WebSession : public NetCore::PacketSession
{
public:
	/*overrides from PacketSession*/
	void OnConnected() override;
	void OnSend(const int len) override;
	void OnDisconnected(const int error) override;
	void OnRecvPacket(const char* buffer, const ushort id) override;

	/*inline-methods*/
	std::shared_ptr<WebSession> SharedFromThis()
	{
		return static_pointer_cast<WebSession>(shared_from_this());
	}

private:
	std::string _remoteIp;
	ushort _port;

	NetCore::HashMap<uint, std::shared_ptr<class GameMap>> _managedMaps;
};

class Server
{
public:
	Server(const std::wstring& ip, const ushort port);
	~Server();

	bool Start();
	bool Stop();

private:
	/*std::shared_ptr<NetCore::Session> _session_factory()
	{
		return NetCore::make_shared<WebSession>();
	}*/

private:
	static constexpr DWORD GET_STATUS_TIMEOUT_MILLISECONDS = 1000;

	std::atomic<bool> _off = false;

	SOCKADDR_IN _addr;
	const ushort _port;
	const std::wstring& _ip;

	std::shared_ptr<NetCore::IOCPCore> _core;
	std::shared_ptr<NetCore::ServerService> _service;
	std::shared_ptr<NetCore::TaskManagerEx> _manager;
};

