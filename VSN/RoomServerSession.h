#pragma once

class RoomServerSession : public NetCore::Session
{
public:
	RoomServerSession() : _port(0) {}

	std::shared_ptr<RoomServerSession> SharedFromThis()
	{
		return static_pointer_cast<RoomServerSession>(shared_from_this());
	}

private:
	void OnConnected() override;
	void OnSend(const NetCore::int32 len) override;
	NetCore::uint32 OnRecv(const NetCore::_byte* buffer, const NetCore::uint32 len) override;
	void OnDisconnected(const NetCore::int32 error) override;

	// Inherited via Session
	void SendRaw(const NetCore::_byte* buffer) override;
	void Send(const NetCore::uint16 id, NetCore::_ubyte* ptr, const NetCore::uint16 size) override;

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
	static constexpr DWORD GET_STATUS_TIMEOUT_MILLISECONDS = 1000;

	std::atomic<bool> _off = false;

	SOCKADDR_IN _addr;
	const ushort _port;
	const std::wstring& _ip;

	std::shared_ptr<NetCore::IOCPCore> _core;
	std::shared_ptr<NetCore::ServerService> _service;
	std::shared_ptr<NetCore::TaskManagerEx> _manager;
};

