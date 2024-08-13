#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::ResponsePacket(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetResponse(buffer);
}

void PacketHandler::ConnectGame(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetConnectGame(buffer);

	if (pkt == nullptr)
	{
		std::cerr << "Wrong pkt [ReqGameInfo]" << std::endl;
		return;
	}
	DebugUtil::Show(pkt);

	session->TryConnectToMap(pkt);
}

void PacketHandler::LoadComplete(const char* buffer, std::shared_ptr<GameSession> session)
{
	const auto pkt = VSN::GetMessagePacket(buffer);
	// TODO
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
