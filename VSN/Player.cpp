#include "pch.h"
#include "Player.h"

Player::Player(uint id, std::weak_ptr<GameSession> session)
	:_id(id), _session(session)
{
}
