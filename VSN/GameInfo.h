#pragma once

// TEMP : All structs can be changed later.

struct PlayerData
{
public:
	uint AccountDbId;
	ushort CharacterId;

	string PlayerAuthToken;
};

struct MapConfiguration
{
public:
	uint MapTypeId;
	std::vector<uint> SpawnableItems;
	ushort Difficulty;
};

struct GameInfoData
{
public:
	uint GameId;
	std::vector<PlayerData> Players;
	MapConfiguration MapConfig;
};

