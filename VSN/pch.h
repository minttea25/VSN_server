#pragma once

#ifndef _DEBUG
#pragma comment(lib, "netcore\\Release\\MyNetworkLib.lib")
#pragma comment(lib, "flatbuffers\\Release\\flatbuffers.lib")
#pragma comment(lib, "glog\\Release\\glog.lib")

#else
#pragma comment(lib, "netcore\\Debug\\MyNetworkLib.lib")
#pragma comment(lib, "flatbuffers\\Debug\\flatbuffers.lib")
#pragma comment(lib, "glog\\Debug\\glogd.lib")

#endif // !_DEBUG

#define GLOG_USE_GLOG_EXPORT /*Definition for glog*/
#define GLOG_NO_ABBREVIATED_SEVERITIES /*Definition for glog*/

#include <iostream>
#include <vector>
#include <functional>
#include <future>

// glog
#include <glog/logging.h>

// Core 
#include "netcore/CoreDef.h"
#include "netcore/CoreMacroDef.h"
#include "netcore/CoreTypes.h"
#include "netcore/CoreGlobal.h"
#include "netcore/CoreTLS.h"
#include "netcore/CoreSTLContainer.h"

#include "netcore/ErrorHandler.h"

#include "netcore/CoreLogger.h"

// Base Headers
#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>
#include <WS2tcpip.h>

// Network Library
#pragma comment(lib, "ws2_32.lib")

// Thread Headers
#include "netcore/TaskManager.h"
#include "netcore/RWLock.h"
#include "netcore/Lock.h"
#include "netcore/ThreadPool.h"
#include "netcore/LockQueue.h"
#include "netcore/LockStack.h"
#include "netcore/LockPriorityQueue.h"

// Memory Headers
#include "netcore/MemoryAllocator.h"
#include "netcore/MemoryPool.h"
#include "netcore/Memory.h"
#include "netcore/ObjectPool.h"

#include "netcore/Utils.h"
#include "netcore/AddrUtils.h"
#include "netcore/SocketUtils.h"

// Network Buffers
#include "netcore/SendBuffer.h"
#include "netcore/RecvBuffer.h"
#include "netcore/SendBufferManager.h"

// Network Headers
#include "netcore/IOCPEvent.h"
#include "netcore/IOCPObject.h"
#include "netcore/IOCPCore.h"
#include "netcore/Connector.h"
#include "netcore/Listener.h"
#include "netcore/Session.h"
#include "netcore/PacketSession.h"
#include "netcore/Service.h"

// Ex
#include "netcore/ClientServiceEx.h"

// Job Headers
#include "netcore/Job.h"
#include "netcore/JobSerializer.h"
#include "netcore/TimeJob.h"
#include "netcore/GlobalTimeJobSerializer.h"
#include "netcore/GlobalJobSerializer.h"
#include "netcore/GlobalJobWorker.h"

// flatbuffers lib Headers
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"

// flatbuffers Headers
#include "netcore/FBAllocator.h"
#include "netcore/PacketWrapper.h"

#include "netcore/TaskManagerEx.h"

#include "netcore/PacketWrapper.h"



// projects
using ushort = unsigned short;
using uint = unsigned int;

#include "Global.h"
#include "Vector2.h"
#include "DataManager.h"
#include "Server.h"
#include "GameInfo.h"
#include "GameObject.h"


#include "RequestParser.h"

#include "fbs_includes.h"


#include "Packet.h"


#include "Player.h"
#include "GameSession.h"
#include "GameMap.h"
#include "GameManager.h"
#include "GameServer.h"

#include "SessionManager.h"
#include "PacketHandler.h"
#include "PacketManager.h"