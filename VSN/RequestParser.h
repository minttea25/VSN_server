#pragma once

enum RequestType : ushort
{
	UNDEFINED_REQ = 0,
	JOIN_LOBBY = 1,
	LOBBY_READY = 2,
	START_LOAD_GAME = 3,
	READY_TO_START = 4,
	START_GAME_ = 5,
};

enum RequestFixedId : ushort
{
	START_TO_LOAD_GAME = 0b1111'1111'1111'1111,
};

class RequestParser
{
	// structure:
	// MSB (2bytes) : request type (enum value)
	// LSB (6bytes) : request id (from client)
public:

	static ushort ReqType(const uint req)
	{
		return (req >> 16) & 0xFFFF;
	}

	static uint ReqId(const uint req)
	{
		return req & 0xFFFF;
	}

	static uint MakeReq(const uint type, const uint id)
	{
		return ((type & 0xFFFF) << 16) | (id & 0xFFFF);
	}
};

