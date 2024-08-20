#pragma once

#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

class Random
{
public:
	template<typename T>
	static void Shuffle(std::vector<T>& v)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(v.begin(), v.end(), std::default_random_engine(seed));
	}

	template<typename T>
	static T GetRandom(const T min, const T max)
	{
		static std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<T> dist(min, max);
		return dist(engine);
	}

	static int GetRandom(const int min, const int max)
	{
		static std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<int> dist(min, max);
		return dist(engine);
	}

	static double GetRandom(const double min, const double max)
	{
		static std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<double> dist(min, max);
		return dist(engine);
	}
};

