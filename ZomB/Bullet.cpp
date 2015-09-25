#pragma once
#include "Bullet.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float v) : 
	_type(NDjinn::CollidableType::Lazy),
	_xywh(glm::vec4(pos, 20, 20)), 
	_dir(dir), 
	_v(v)
{
}

Bullet::~Bullet()
{
	_collider->removeCollidable(this);
}

void Bullet::init(NDjinn::QNode* collider) {
	_collider = collider;
	_tex = NDjinn::AssetManager::getTexture("assets/hero/kamehameha.png");
	_color.r = 255; _color.g = 255; _color.b = 255, _color.a = 255;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	_collider->addCollidable(this);
}

void Bullet::draw(NDjinn::SpriteBatch& sprite) {
	sprite.draw(_xywh, _uv, _tex.id, 0.0f, _color);
}

bool Bullet::update(NDjinn::Camera2D& cam) {
	glm::vec4 dirVec(_dir * _v, 0.0f, 0.0f);
	bool isMoving = (dirVec.x > 0.0f || dirVec.y > 0.0f);
	glm::vec2 newPos(_xywh.x + dirVec.x, _xywh.y + dirVec.y);
	//getCollidables(*this, _collidables);

	_xywh += dirVec;

	_collider->updateCollidable(this, isMoving);
	return !cam.isInView(glm::vec2(_xywh.x, _xywh.y));
}

glm::vec4 Bullet::getDims() { return _xywh; }

NDjinn::CollidableType Bullet::getType()
{
	return _type;
}
