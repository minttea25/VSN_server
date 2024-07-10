#include "pch.h"

const std::string LOGDIR("../Logs");
const std::wstring IP(L"127.0.0.1");
constexpr ushort PORT = 8080;
constexpr int SESSION_COUNT = 100;
constexpr int BACK_LOG = 100;

constexpr ushort WAITING_WEB_PORT = 8070;

int main(int argc, char* argv)
{
	GDataManager = NetCore::make_shared<DataManager>();


	NetCore::InitNetCore(argv, LOGDIR);

	{
		google::LogToStderr();
	}

	auto web = new Server(IP, WAITING_WEB_PORT);
	if (!web->Start())
	{
		delete web;
		return -1;
	}

	auto server = new GameServer(IP, PORT, SESSION_COUNT, BACK_LOG);

	if (!server->Start())
	{
		delete server;
		return -1;
	}

	server->StartWork(4);

	while (true)
	{
		std::string cmd;
		getline(cin, cmd);

		if (strcmp(cmd.c_str(), "stop"))
		{
			if (server->Stop())
			{
				// error
			}
			break;
		}
	}

	delete server;
	delete web;

	return 0;
}