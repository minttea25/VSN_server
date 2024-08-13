#include "pch.h"
#include "DebugUtil.h"

const std::string LOGDIR("../Logs");
const std::wstring IP(L"127.0.0.1");
constexpr ushort PORT = 8080;
constexpr int SESSION_COUNT = 100;
constexpr int BACK_LOG = 100;

constexpr ushort WAITING_WEB_PORT = 8070;

int main(int argc, char* argv)
{
	GDataManager = std::make_shared<DataManager>();

	{
		std::vector<uint8_t> buf;
		GDataManager->LoadMapData(1, buf);
		auto map_data = VSN::GetMapData(buf.data());
		//DebugUtil::Show(map_data);

	}
	{
		std::vector<uint8_t> buf;
		GDataManager->LoadMapData(2, buf);
		auto map_data = VSN::GetMapData(buf.data());
		//DebugUtil::Show(map_data);
	}

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