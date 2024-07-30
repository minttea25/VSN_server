#include "pch.h"
#include "Player.h"

Player::Player(const uint nid, const PlayerData data)
	: GameObject(nid), _nid(nid)
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
