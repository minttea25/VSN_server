#pragma once
class PacketHandler
{
	static constexpr uint INVALID_REQ_ID = 0;
public:
	static void ResponsePacket(const char* buffer, std::shared_ptr<GameSession> session);

	static void ConnectGame(const char* buffer, std::shared_ptr<GameSession> session);
	static void LoadComplete(const char* buffer, std::shared_ptr<GameSession> session);

	static void ReqGetExpPacket(const char* buffer, std::shared_ptr<GameSession> session);
	static void ReqGetItemPacket(const char* buffer, std::shared_ptr<GameSession> session);;
	static void ReqMonsterDamagedPacket(const char* buffer, std::shared_ptr<GameSession> session);

};

