#include "pch.h"
#include "Global.h"
#include "GameManager.h"
#include "PacketManager.h"
#include "SessionManager.h"
#include "DataManager.h"

std::shared_ptr<class SessionManager> GSessionManager = nullptr;
std::shared_ptr<class PacketManager> GPacketManager = nullptr;
std::shared_ptr<class GameManager> GGameManager = nullptr;
std::shared_ptr<class DataManager> GDataManager = nullptr;