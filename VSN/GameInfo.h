#pragma once

// TEMP : All structs can be changed later.

struct PlayerData
{
public:
	uint AccountDbId;
	ushort CharacterId;

	string PlayerAuthToken;

	PlayerData(const uint& AccountDbId, const ushort& CharacterId, const string& PlayerAuthToken)
		: AccountDbId(AccountDbId), CharacterId(CharacterId), PlayerAuthToken(PlayerAuthToken)
	{
	}
};

struct MapConfiguration
{
public:
	uint MapTypeId;
	std::vector<uint> SpawnableItems;
	ushort Difficulty;

	MapConfiguration(const uint& MapTypeId, const std::vector<uint>& SpawnableItems, const ushort& Difficulty)
		: MapTypeId(MapTypeId), SpawnableItems(SpawnableItems), Difficulty(Difficulty)
	{
	}
};

struct GameInfoData
{
public:
	uint GameId;
	std::vector<PlayerData> Players;
	MapConfiguration MapConfig;

	GameInfoData(const uint& GameId, const std::vector<PlayerData>& Players, const MapConfiguration& MapConfig)
		: GameId(GameId), Players(Players), MapConfig(MapConfig)
	{
	}
};

