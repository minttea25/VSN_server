#include "pch.h"
#include "DataManager.h"

DataManager::DataManager()
{
    std::vector<std::string> jsons { MAP_1, MAP_2 };
    std::vector<std::string> bins { "MapData1.mapdata", "MapData2.mapdata" };
    SaveBinFromJson(jsons, "../fbs/MapData.fbs", bins, 2, true);
}

DataManager::~DataManager()
{
}

void DataManager::LoadMapData(const uint mapTypeId, OUT std::vector<uint8_t>& buf)
{
    std::string path = "MapData" + std::to_string(mapTypeId) + ".mapdata";
    if (std::filesystem::exists(path) == false)
    {
        std::cerr << "Can not find: " << path << std::endl;
        return;
    }

    std::cerr << "Loading MapData: " << path << std::endl;

    std::ifstream in_file(path, std::ios::binary);
    buf = std::vector<uint8_t>((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());
}


bool DataManager::CheckFiles()
{

    return false;
}


const uint8_t* DataManager::LoadDataFromBin(const std::string& bin_file)
{
    // Load the generated FlatBuffer and access its contents
    std::ifstream in_file(bin_file, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());

    return buffer.data();
}

void DataManager::SaveBinFromJson(const std::string& json_file, const std::string& fbs_file, const std::string& output_file, const bool recreate)
{
    if (recreate == false && std::filesystem::exists(output_file))
    {
        return;
    }

    // Load the schema (.fbs) file
    std::string schema_file;
    if (!flatbuffers::LoadFile(fbs_file.c_str(), false, &schema_file))
    {
        std::cerr << "Failed to load schema file: " << fbs_file << std::endl;
        return;
    }

    // Parse the schema
    flatbuffers::Parser parser;
    if (!parser.Parse(schema_file.c_str()))
    {
        std::cerr << "Failed to parse schema: " << parser.error_ << std::endl;
        return;
    }

    // Load the JSON file
    std::string json_data;
    if (!flatbuffers::LoadFile(json_file.c_str(), false, &json_data))
    {
        std::cerr << "Failed to load JSON file: " << json_file << std::endl;
        return;
    }

    // Parse the JSON data into a FlatBuffer
    if (!parser.Parse(json_data.c_str()))
    {
        std::cerr << "Failed to parse JSON: " << parser.error_ << std::endl;
        return;
    }

    // Save the FlatBuffer to a file
    const uint8_t* buffer = parser.builder_.GetBufferPointer();
    int size = parser.builder_.GetSize();
    std::ofstream out_file(output_file, std::ios::binary);
    out_file.write(reinterpret_cast<const char*>(buffer), size);
    out_file.close();

    std::cerr << "Created: " << output_file << std::endl;
}

void DataManager::SaveBinFromJson(const std::vector<std::string>& json_files, const std::string& fbs_file, const std::vector<std::string>& output_files, const int count, const bool recreate)
{
    if (json_files.size() > count)
    {
        std::cerr << "The size of json_files does not correspond with the parameter, count." << std::endl;
        return;
    }
    if (output_files.size() > count)
    {
        std::cerr << "The size of output_files does not correspond with the parameter, count." << std::endl;
        return;
    }

    // Load the schema (.fbs) file
    std::string schema_file;
    if (!flatbuffers::LoadFile(fbs_file.c_str(), false, &schema_file))
    {
        std::cerr << "Failed to load schema file: " << fbs_file << std::endl;
        return;
    }

    // Parse the schema
    flatbuffers::Parser parser;
    if (!parser.Parse(schema_file.c_str()))
    {
        std::cerr << "Failed to parse schema: " << parser.error_ << std::endl;
        return;
    }

    for (int i = 0; i < count; ++i)
    {
        if (recreate == false && std::filesystem::exists(output_files[i]))
        {
            continue;
        }

        // Load the JSON file
        std::string json_data;
        if (!flatbuffers::LoadFile(json_files[i].c_str(), false, &json_data))
        {
            std::cerr << "Failed to load JSON file: " << json_files[i] << std::endl;
            continue;
        }

        // Parse the JSON data into a FlatBuffer
        if (!parser.Parse(json_data.c_str()))
        {
            std::cerr << "Failed to parse JSON: " << parser.error_ << std::endl;
            continue;
        }

        // Save the FlatBuffer to a file
        const uint8_t* buffer = parser.builder_.GetBufferPointer();
        int size = parser.builder_.GetSize();
        std::ofstream out_file(output_files[i], std::ios::binary);
        out_file.write(reinterpret_cast<const char*>(buffer), size);
        out_file.close();

        std::cerr << "Created: " << output_files[i] << std::endl;
    }
}
