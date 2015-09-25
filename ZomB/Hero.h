#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <NDjinn\AssetManager.h>
#include <NDjinn\Vertex.h>
#include <NDjinn\SpriteBatch.h>
#include <NDjinn\Camera2D.h>
#include <NDjinn\GLTexture.h>
#include <NDjinn\QNode.h>
#include <NDjinn\ICollidable.h>

#include "Bullet.h"

class Hero : public NDjinn::ICollidable{
private:
	NDjinn::CollidableType _type;
	glm::vec4 _xywh;
	NDjinn::GLTexture _tex;
	glm::vec4 _uv;
	NDjinn::Color _color;
	float _speed;

	std::vector<Bullet> _bullets;
	float _bulletSpeed;

	NDjinn::QNode* _collider;
	std::set<ICollidable*> _collidables;


	void updateBullets(NDjinn::Camera2D& cam);
public:
	Hero();
	~Hero();

	void init(NDjinn::QNode* collider);
	bool update(NDjinn::Camera2D& cam); // returns true if killed
	void shoot(glm::vec2 dir);
	void draw(NDjinn::SpriteBatch& sprites);
	void move(glm::vec2 dir);

	//getters
	glm::vec2 getPos() { return glm::vec2(_xywh.x, _xywh.y); }
	float getSpeed() { return _speed; }

	//setters
	void setPos(float x, float y) { _xywh.x = x; _xywh.y = y; }
	void setSpeed(float mult) { _speed *= mult; }

	// via ICollidable
	virtual glm::vec4 getDims() override;
	virtual NDjinn::CollidableType getType() override;
};

