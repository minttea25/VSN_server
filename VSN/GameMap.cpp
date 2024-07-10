#include "pch.h"
#include "GameMap.h"

GameMap::GameMap(GameInfoData data)
	: _gameId(data.GameId)
{

}

GameMap::~GameMap()
{
}

void GameMap::Init()
{
	NetCore::GGlobalJobWorker->AddTimeJobQueue(shared_from_this());
}

void GameMap::StartGame()
{
}