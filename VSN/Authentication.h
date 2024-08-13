#pragma once
class Authentication
{
	// `flag` is a value which can be get from shared-database. 
	// It should be get from the database at each platform.
	// Note: the client and server must have same value as key.
	const unsigned long long flag = 1;

public:
	static std::string CreateAuthToken(const std::string key, const uint id)
	{


		// TODO
		return "";
	}
};

