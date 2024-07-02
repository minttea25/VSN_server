#include "pch.h"

const std::string LOGDIR("../Logs");
const std::wstring IP(L"127.0.0.1");
constexpr ushort PORT = 8080;
constexpr int SESSION_COUNT = 100;
constexpr int BACK_LOG = 100;

int main(int argc, char* argv)
{
	NetCore::InitNetCore(argv, LOGDIR);

	{
		google::LogToStderr();
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

	return 0;
}