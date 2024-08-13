#pragma once

class PacketManager
{
public:
	PacketManager() { _init(); }
	PacketManager(const PacketManager&) = delete; // copy
	PacketManager(PacketManager&&) = delete; // move

	inline void Dispatch(const ushort id, const char* ptr, std::shared_ptr<GameSession> session)
	{
		auto it = _handlers.find(id);
		if (it != _handlers.end()) it->second(ptr, session);
		else
		{
			LOG(INFO) << "No registered id: " << id;
		}
	}
private:
	void _init()
	{
		_handlers.insert({ RESPONSE, PacketHandler::ResponsePacket });

		_handlers.insert({ REQ_GAME_INFO, PacketHandler::ConnectGame });
		_handlers.insert({ LOAD_COMPLETE, PacketHandler::LoadComplete });

		_handlers.insert({ REQ_GET_EXP, PacketHandler::ReqGetExpPacket });
		_handlers.insert({ REQ_GET_ITEM, PacketHandler::ReqGetItemPacket });
		_handlers.insert({ REQ_MONSTER_DAMAGED, PacketHandler::ReqMonsterDamagedPacket });

	}

private:
	std::map<ushort, void(*)(const char*, std::shared_ptr<GameSession>)> _handlers;
};

