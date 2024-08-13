#pragma once 

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject(const uint nid) : _nid(nid), _speed(0) {}
	uint Nid() const { return _nid; }
	Vector2 Position() const { return _position; }
	void SetPosition(const float x, const float y) { _position.x = x; _position.y = y; }
protected:
	uint _nid;
	Vector2 _position;
	Vector2 _dir;
	float _speed;
};

class StaticGameObject : public enable_shared_from_this<StaticGameObject>
{
public:
	StaticGameObject(const uint nid) : _nid(nid) {}
	uint Nid() const { return _nid; }
protected:
	uint _nid;
	Vector2 _position;
};