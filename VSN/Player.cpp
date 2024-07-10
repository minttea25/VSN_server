#include "pch.h"
#include "Player.h"

Player::Player(weak_ptr<GameSession> session, const uint nid, const uint characterTypeId)
	: GameObject(nid), _session(session)
{
	// TEMP
	_hp = 300;
}

Player::~Player()
{
}

void Player::Update(VSN::PlayerState state)
{
}
