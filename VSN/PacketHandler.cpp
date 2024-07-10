#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::RequestPacket(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetRequest(buffer);
	if (pkt == nullptr)
	{
		const auto res = Packets::WrongReq(INVALID_REQ_ID);
		session->Send(res.id, res.Buf(), res.size);
		return;
	}

	const ushort type = RequestParser::ReqType(pkt->req_id());
	switch (type)
	{
	// TEST
	case 9999:
	{
		auto pp = Packets::Response(pkt->req_id(), 1);
		session->Send(pp.id, pp.Buf(), pp.size);
	}
	{
		NetCore::Vector<VSN::MonsterSpawnInfo> info;
		info.push_back(VSN::MonsterSpawnInfo(1, 1, { 1, 2 }));
		info.push_back(VSN::MonsterSpawnInfo(2, 1, { -3, 5 }));
		info.push_back(VSN::MonsterSpawnInfo(3, 1, { 3, -2 }));
		info.push_back(VSN::MonsterSpawnInfo(4, 1, { 3, 3 }));
		info.push_back(VSN::MonsterSpawnInfo(5, 1, { -1, -2 }));
		auto pp = Packets::MonsterSpawn(info);
		session->Send(pp.id, pp.Buf(), pp.size);
	}
		break;
	default:
		// TODO : wrong reqeust
		LOG(INFO) << "Wrong request: id=" << type;
		break;
	}
}

void PacketHandler::ResponsePacket(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetResponse(buffer);
}

void PacketHandler::ReqGetExpPacket(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetReq_GetExp(buffer);
}

void PacketHandler::ReqGetItemPacket(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetReq_GetItem(buffer);
}

void PacketHandler::ReqMonsterDamagedPacket(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetReq_MonsterDamaged(buffer);
}

void PacketHandler::ReqPlayerDamagedPacket(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetReq_PlayerDamaged(buffer);
}

void PacketHandler::ReqPlayerUpgradePacket(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetReq_PlayerUpgrade(buffer);
}
