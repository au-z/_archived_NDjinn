#pragma once
#include <iostream>
#include <set>
#include "Hero.h"

Hero::Hero() :
	_xywh(0.0f, 0.0f, 50.0f, 50.0f),
	_bulletSpeed(8.0f),
	_speed(4.0f)
{
}

Hero::~Hero(){}

void Hero::init(NDjinn::QNode* collider) {
	//TODO: DI?
	_collider = collider;
	_tex = NDjinn::AssetManager::getTexture("assets/hero/hero.png");
	_color.r = 255; _color.g = 255; _color.b = 255; _color.a = 255;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	registerCollidable();
}

void Hero::shoot(glm::vec2 dir) {
	_bullets.emplace_back(getPos(), dir, _bulletSpeed, BULLET_LIFE);
}

void Hero::move(glm::vec2 dir) {
	glm::vec2 newPos(_xywh.x += dir.x, _xywh.y += dir.y);
	_collider->getCollidables(this, newPos, true, &_collidables);
	//std::cout << "collider count = " << _collidables.size() << std::endl;

	//todo, react to collisions
	_xywh.x += dir.x;
	_xywh.y += dir.y;

	//std::cout << "Hero: " << _xywh.x << "," << _xywh.y << std::endl;
}

bool Hero::update(NDjinn::Camera2D& cam) {
	updateBullets(cam);
	_collider->updateCollidable(this);
	_collidables.empty(); //clear set
	return false;
}

void Hero::updateBullets(NDjinn::Camera2D& cam) {
	for (unsigned int i = 0; i < _bullets.size();) {
		if (_bullets[i].update(cam) == true) {
			_bullets[i] = _bullets.back(); // swap with back of vector
			_bullets.pop_back();
		}
		else {
			++i; // only increment if bullet is not destroyed
		}
	}
}

void Hero::draw(NDjinn::SpriteBatch& sprites) {
	sprites.draw(_xywh, _uv, _tex.id, 0.0f, _color);
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(sprites);
	}
}

void Hero::registerCollidable() { _collider->addCollidable(this); }

void Hero::getCollidables(glm::vec2 newPos, bool isMoving, std::set<ICollidable*>* collidables) {
	_collider->getCollidables(this, newPos, isMoving, collidables);
}

glm::vec4 Hero::getDims() { return _xywh; }