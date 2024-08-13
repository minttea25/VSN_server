#pragma once
class GameServer : public NetCore::JobSerializer
{
public:
	GameServer(const std::wstring& ip, const ushort port, const ushort sessionCount, const ushort backlog);

	~GameServer();

	bool Start();

	bool Stop();

	void StartWork(const int threadCount);

private:
	void _netCoreWork();

private:
	static constexpr DWORD GET_STATUS_TIMEOUT_MILLISECONDS = 100;
	static constexpr unsigned long long RESERVED_JOB_DURATION = 100;

	std::atomic<bool> _off = false;

	SOCKADDR_IN _addr;
	const ushort _port;
	const std::wstring& _ip;

	std::shared_ptr<NetCore::IOCPCore> _core;
	std::shared_ptr<NetCore::ServerService> _service;
	std::shared_ptr<NetCore::TaskManagerEx> _manager;
	std::function<std::shared_ptr<NetCore::Session>()> _session_factory;
};

