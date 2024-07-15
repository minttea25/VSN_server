#include "pch.h"
#include "Player.h"

Player::Player(const uint nid, const PlayerData data)
	: GameObject(nid), _data(data), _nid(nid), _ready(false)
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
