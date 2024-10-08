#pragma once

enum PacketId : ushort
{
    NONE = 0,

    // Common 1 ~
    REQUEST = 1,
    RESPONSE = 2,
    SIMPLE = 3,

    // Game 100 ~
    ITEM_SPAWN = 102,

    // Monster 200 ~
    MONSTER_SPAWN = 201,
    MONSTER_DAMAGED = 202,
    MONSTER_DEAD = 203,
    MONSTER_STATE = 204,

    // Player 300 ~
    PLAYER_STATE = 301,
    PLAYER_DAMAGED = 302,
    PLAYER_LEVEL_UP = 303,
    PLAYER_GET_ITEM = 304,
    PLAYER_UPGRADE = 305,

    // Req from client 1000 ~
    REQ_GET_EXP = 1001,
    REQ_GET_ITEM = 1002,
    REQ_MONSTER_DAMAGED = 1003,
    REQ_PLAYER_DAMAGED = 1004,
    REQ_PLAYER_UPGRADE = 1005,

    // Res to client 2000 ~
    RES_GET_EXP = 2001,

};

enum SimpleId
{
    SP_GAME_START = 1,
};

enum ResId
{
    INTERNAL_ERROR = -2,
    WRONG_REQ = -1,
    NO_RES = 0,
    OK = 1,

};

struct Packet
{
public:
    Packet(const ushort id, const uint size, uint8_t* ptr)
        : id(id), size(size), ptr(ptr) {}

    uint8_t* Buf() const { return ptr; }

    const ushort id;
    const uint size;
private:
    uint8_t* ptr;
};

#define __BUILDER(size)                                 \
NetCore::FBAllocator allocator;                         \
flatbuffers::FlatBufferBuilder fb(size, &allocator);    \
\

#define __PACKET(id) Packet(id, fb.GetSize(), fb.GetBufferPointer())

class Packets
{
public:
    static Packet Simple(const uint id, const std::string& msg)
    {
        __BUILDER(128);
        auto msg_os = fb.CreateString(msg);
        auto pkt_os = VSN::CreateSimplePacket(fb, id, msg_os);
        fb.Finish(pkt_os);

        return __PACKET(SIMPLE);

    }

    static Packet Request(const uint req_id)
    {
        NetCore::FBAllocator allocator;
        flatbuffers::FlatBufferBuilder fb(64, &allocator);
        auto pkt_os = VSN::CreateRequest(fb, req_id);
        fb.Finish(pkt_os);

        return Packet{ REQUEST, fb.GetSize(), fb.GetBufferPointer() };
    }

    static Packet Response(const uint req_id, const short res)
    {
        __BUILDER(64);
        auto pkt_os = VSN::CreateResponse(fb, req_id, res);
        fb.Finish(pkt_os);

        return __PACKET(RESPONSE);
    }

    static Packet MonsterSpawn(const NetCore::Vector<VSN::MonsterSpawnInfo>& infos)
    {
        __BUILDER(1024);

        auto info_ofs = fb.CreateVectorOfStructs(infos);
        auto pkt_os = VSN::CreateSpawnMonsters(fb, info_ofs);
        fb.Finish(pkt_os);

        return __PACKET(MONSTER_SPAWN);
    }

    static Packet MonsterDamaged(const NetCore::Vector<VSN::DamagedMonsterInfo>& infos)
    {
        __BUILDER(1024);

        auto info_ofs = fb.CreateVectorOfStructs(infos);
        auto pkt_os = VSN::CreateMonsterDamaged(fb, info_ofs);
        fb.Finish(pkt_os);

        return __PACKET(MONSTER_DAMAGED);
    }

    static Packet MonsterDead(const NetCore::Vector<VSN::MonsterDeadInfo>& infos)
    {
        __BUILDER(512);

        auto info_ofs = fb.CreateVectorOfStructs(infos);
        auto pkt_os = VSN::CreateMonsterDead(fb, info_ofs);
        fb.Finish(pkt_os);

        return __PACKET(MONSTER_DEAD);
    }

    static Packet MonsterState(const NetCore::Vector<VSN::MonsterStateInfo>& infos)
    {
        __BUILDER(1024);

        auto info_ofs = fb.CreateVectorOfStructs(infos);
        auto pkt_ofs = VSN::CreateMonsterStates(fb, info_ofs);
        fb.Finish(pkt_ofs);

        return __PACKET(MONSTER_STATE);
    }

    static Packet PlayerState(const uint playerId, const uint changeFlag, 
        const bool towardLeft, const VSN::Vector2& pos, const unsigned char state)
    {
        __BUILDER(256);

        const VSN::Vector2* ppos = (changeFlag | (1u << 1)) ? &pos : nullptr;


        auto pkt_ofs = VSN::CreatePlayerState(fb, playerId, changeFlag,
            towardLeft, ppos, state);
        fb.Finish(pkt_ofs);

        return __PACKET(PLAYER_STATE);
    }

    static Packet PlayerDamaged(const uint player_id, const int damage)
    {
        __BUILDER(64);

        auto pkt_ofs = VSN::CreatePlayerDamaged(fb, player_id, damage);
        fb.Finish(pkt_ofs);

        return __PACKET(PLAYER_DAMAGED);
    }

    static Packet PlayerLevelUp(const uint player_id, const ushort now_level)
    {
        __BUILDER(64);

        auto pkt_ofs = VSN::CreatePlayerLevelUp(fb, player_id, now_level);
        fb.Finish(pkt_ofs);

        return __PACKET(PLAYER_LEVEL_UP);
    }

    static Packet PlayerGetItem(const uint player_id, const uint item_nid, const unsigned char item_type)
    {
        __BUILDER(64);

        auto pkt_ofs = VSN::CreatePlayerGetItem(fb, player_id, item_nid, item_type);
        fb.Finish(pkt_ofs);

        return __PACKET(PLAYER_GET_ITEM);
    }

    static Packet PlayerUpgrade_Skill(const uint player_id, const unsigned char skill_type, const ushort value)
    {
        __BUILDER(128);

        auto upgrade_ofs = VSN::CreateSkillUpgrade(fb, skill_type, value);
        auto pkt_ofs = VSN::CreatePlayerUpgrade(fb, player_id, VSN::UpgradeInfo_SkillUpgrade, flatbuffers::Offset<void>(upgrade_ofs.o));
        fb.Finish(pkt_ofs);

        return __PACKET(PLAYER_UPGRADE);
    }

    static Packet PlayerUpgrade_Stat(const uint player_id, const unsigned char stat_type, const ushort value)
    {
        __BUILDER(128);

        auto upgrade_ofs = VSN::CreateStatUpgrade(fb, stat_type, value);
        auto pkt_ofs = VSN::CreatePlayerUpgrade(fb, player_id, VSN::UpgradeInfo_StatUpgrade, flatbuffers::Offset<void>(upgrade_ofs.o));
        fb.Finish(pkt_ofs);

        return __PACKET(PLAYER_UPGRADE);
    }

    static Packet ResGetExp(const uint req_id, const uint exp_item_id)
    {
        __BUILDER(64);

        auto pkt_ofs = VSN::CreateRes_GetExp(fb, req_id, exp_item_id);
        fb.Finish(pkt_ofs);

        return __PACKET(RES_GET_EXP);
    }

    static Packet WrongReq(const uint req_id)
    {
        __BUILDER(64);

        const auto pkt_ofs = VSN::CreateResponse(fb, req_id, -1);
        fb.Finish(pkt_ofs);

        return __PACKET(REQUEST);
    }
};

