#pragma once
#include <vector>
#include <NDjinn\Vertex.h>
#include <NDjinn\GLTexture.h>
#include <NDjinn\Camera2D.h>
#include <NDjinn\SpriteBatch.h>
#include <NDjinn\QNode.h>
#include <NDjinn\ICollidable.h>
#include <glm\glm.hpp>

class Peasant : public NDjinn::ICollidable{
private:
	const int SMOOTHING_COUNT = 20;

	NDjinn::CollidableType _type;
	glm::vec4 _xywh;
	NDjinn::GLTexture _tex;
	glm::vec4 _uv;
	NDjinn::Color _color;
	float _speed;
	bool _isZombie;

	std::vector<glm::vec2> _prevDirs;

	NDjinn::QNode* _collider;

	void setTex();

public:
	Peasant();
	~Peasant();

	void init(NDjinn::QNode* collider, bool isZombie); // default to false
	bool update(glm::vec2 heroPos); // returns true if killed
	void draw(NDjinn::SpriteBatch& sprites);
	void move(glm::vec2 dir);

	//getters
	glm::vec2 getPos() { return glm::vec2(_xywh.x, _xywh.y); }
	float getSpeed() { return _speed; }
	bool isZombie() { return _isZombie; }

	//setters
	void setPos(float x, float y) { _xywh.x = x; _xywh.y = y; }

	// Inherited via ICollidable
	virtual glm::vec4 getDims() override;
	virtual NDjinn::CollidableType getType() override;
};

