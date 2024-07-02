#include "pch.h"
#include "GameMap.h"

GameMap::GameMap() : _gameId(UNSIGNED_INVALID), _readySet(0)
{
}

GameMap::~GameMap()
{
}

void GameMap::Init()
{
	// TODO : get unique game id
	// TEMP
	_gameId = __TEST_MAP_ID;

	NetCore::GGlobalJobWorker->AddTimeJobQueue(shared_from_this());
}

void GameMap::StartGame()
{
	// TODO
}

void GameMap::_playerJoin(std::shared_ptr<Player> player, const uint req)
{
	ushort id = 0;
	for (int i = 1; i <= MAX_PLAYER; ++i)
	{
		if (_players[i] == nullptr)
		{
			id = i;
			player->InMapId = id;
			_players[i] = player->shared_from_this();
		}
	}

	if (id != 0)
	{
		const auto pkt = Packets::Response(req, ResId::OK);
		player->Session().lock()->Send(pkt.id, pkt.Buf(), pkt.size);
	}
	else
	{
		const auto pkt = Packets::Response(req, ResId::INTERNAL_ERROR);
		player->Session().lock()->Send(pkt.id, pkt.Buf(), pkt.size);
	}
}

void GameMap::_playerReady(std::shared_ptr<Player> player, const uint req)
{
	const ushort inMapId = player->InMapId;
	// Check the player is valid
	if (inMapId > 0 && inMapId <= MAX_PLAYER && _players[inMapId - 1] != nullptr)
	{
		// if valid, set ready
		SetReady(inMapId);

		const auto pkt = Packets::Response(req, ResId::OK);
		player->Session().lock()->Send(pkt.id, pkt.Buf(), pkt.size);

		// check all player ready
		if (CheckAllPlayerReady())
		{
			// if all players are ready, send to start to load packet after x ticks
			ReserveJob(START_COUNTDOWN, &GameMap::_startLoadGame);
		}
	}
	else
	{
		const auto pkt = Packets::Response(req, ResId::INTERNAL_ERROR);
		player->Session().lock()->Send(pkt.id, pkt.Buf(), pkt.size);
	}
}

void GameMap::_startLoadGame()
{
	const auto req = RequestParser::MakeReq(RequestType::UNDEFINED_REQ, RequestFixedId::START_TO_LOAD_GAME);
	for (auto& p : _players)
	{
		const auto pkt = Packets::Request(req);
		p->Session().lock()->Send(pkt.id, pkt.Buf(), pkt.size);
	}
}
