#pragma once

struct VSN::MapData;

class DataManager
{
	const char* MAP_1 = "../data/Map_1.json";
	const char* MAP_2 = "../data/Map_2.json";

public:
	DataManager();
	~DataManager();

	void LoadMapData(const uint mapTypeId, OUT std::vector<uint8_t>& buf);
	

private:
	bool CheckFiles();


	const uint8_t* LoadDataFromBin(const std::string& bin_file);
	void SaveBinFromJson(const std::string& json_file, const std::string& fbs_file, const std::string& output_file, const bool recreate = false);
	void SaveBinFromJson(const std::vector<std::string>& json_files, const std::string& fbs_file, const std::vector<std::string>& output_files, const int count, const bool recreate = false);

};

