#pragma once
class DebugUtil
{
public:
#ifdef _DEBUG
	static void Show(const VSN::MapData* map_data)
	{
		if (map_data == nullptr)
		{
			std::cerr << "map_data is nullptr." << std::endl;
			return;
		}

		std::cout << "MapType: " << map_data->map_type() << std::endl;
		std::cout << "MapSize: (" << map_data->map_size()->x() << ", " << map_data->map_size()->y() << ")" << std::endl;

		auto initial_spawn_point = map_data->initial_player_spawnpoint();
		for (size_t i = 0; i < initial_spawn_point->size(); ++i)
		{
			std::cout << "InitialSpawnPoint[" << i << "]: ("
				<< initial_spawn_point->Get(i)->x() << ", "
				<< initial_spawn_point->Get(i)->y() << ")" << std::endl;
		}

		auto item_spawn_schedule1 = map_data->item_spawn_schedule();
		for (size_t i = 0; i < item_spawn_schedule1->size(); ++i)
		{
			std::cout << "ItemSpawnSchedule[" << i << "]: "
				<< "time=" << item_spawn_schedule1->Get(i)->time() << ", "
				<< "position=(" << item_spawn_schedule1->Get(i)->position().x() << ", "
				<< item_spawn_schedule1->Get(i)->position().y() << "), "
				<< "typeId=" << item_spawn_schedule1->Get(i)->type_id() << std::endl;
		}

		auto monster_spawn_schedule1 = map_data->monster_spawn_schedule();
		for (size_t i = 0; i < monster_spawn_schedule1->size(); ++i)
		{
			std::cout << "MonsterSpawnSchedule[" << i << "]: "
				<< "time=" << monster_spawn_schedule1->Get(i)->time() << ", "
				<< "position=(" << monster_spawn_schedule1->Get(i)->position().x() << ", "
				<< monster_spawn_schedule1->Get(i)->position().y() << "), "
				<< "typeId=" << monster_spawn_schedule1->Get(i)->type_id() << std::endl;
		}
	}

	static void Show(const VSN::WebGameInfoData* data)
	{
		if (data == nullptr)
		{
			std::cerr << "data is nullptr." << std::endl;
			return;
		}

		const auto p_size = data->player_account_db_id()->size();
		const auto c_size = data->player_character_type()->size();
		const auto a_size = data->player_auth_token()->size();

		if (p_size != c_size || c_size != a_size)
		{
			std::cerr << "the size of player data does not match.";
			return;
		}

		std::cout << "Game Id: " << data->game_id() <<
			"\nMap Type Id: " << data->map_type_id() <<
			"\nDifficulty: " << (uint)(data->difficulty()) <<
			"\nPlayer Count: " << p_size <<
			"\nPlayers\n";

		for (int i=0; i<p_size; ++i)
		{
			const auto id = data->player_account_db_id()->Get(i);
			const auto character = data->player_character_type()->Get(i);
			const auto auth = data->player_auth_token()->Get(i)->c_str();

			std::cout << "[" << id << "]: " << character << ", " << auth << '\n';
		}

		const auto iSize = data->spawnable_items()->size();
		std::cout << "Spawnable Items: ";
		for (int i = 0; i < iSize; ++i)
		{
			std::cout << data->spawnable_items()->Get(i) << ", ";
		}
		std::cout << std::endl;
	}
#else
	static void Show(const VSN::MapData* map_data) {}
	static void Show(const VSN::WebGameInfoData* data) {}
#endif
};

