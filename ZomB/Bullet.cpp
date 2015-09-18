#pragma once
#include "Bullet.h"
#include <iostream>
#include <glm/glm.hpp>
#include <NDjinn\AssetManager.h>
#include <NDjinn\GLTexture.h>
#include <NDjinn\Window.h>

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float v, NDjinn::QNode* collider) : _xywh(glm::vec4(pos, 20, 20)), _dir(dir), _v(v)
{
	init(collider);
}

Bullet::~Bullet()
{
}

void Bullet::init(NDjinn::QNode* collider) {
	_collider = collider;
	registerCollidable();
}

void Bullet::draw(NDjinn::SpriteBatch& sprite) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static NDjinn::GLTexture tex = NDjinn::AssetManager::getTexture("assets/hero/kamehameha.png");
	NDjinn::Color c; c.r = 255; c.g = 255; c.b = 255, c.a = 255;
	sprite.draw(_xywh, uv, tex.id, 0.0f, c); //create new bullet sprite
}

bool Bullet::update(NDjinn::Camera2D& cam) {
	glm::vec4 dirVec(_dir * _v, 0.0f, 0.0f);
	glm::vec2 newPos(_xywh.x + dirVec.x, _xywh.y + dirVec.y);
	_collider->getCollidables(this, newPos, true, &_collidables);

	_xywh += dirVec; 

	_collider->updateCollidable(this);
	return !cam.isInView(glm::vec2(_xywh.x, _xywh.y));
}

void Bullet::registerCollidable() { _collider->addCollidable(this); }

void Bullet::getCollidables(glm::vec2 newPos, bool isMoving, std::set<ICollidable*>* collidables)
{
	_collider->getCollidables(this, newPos, isMoving, collidables);
}

glm::vec4 Bullet::getDims()
{
	return _xywh;
}
