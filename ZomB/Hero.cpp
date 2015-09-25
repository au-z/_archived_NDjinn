#pragma once
#include <iostream>
#include <set>
#include "Hero.h"

Hero::Hero() :
	_type(NDjinn::CollidableType::Strict),
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
	_collider->addCollidable(this);
}

void Hero::shoot(glm::vec2 dir) {
	Bullet b(getPos(), dir, _bulletSpeed);
	b.init(_collider);
	_bullets.push_back(b);
}

void Hero::move(glm::vec2 dir) {
	//glm::vec2 newPos(_xywh.x += dir.x, _xywh.y += dir.y);
	_collider->getCollidables(this, _collidables);

	//todo, react to collisions
	_xywh.x += dir.x;
	_xywh.y += dir.y;
	_collider->updateCollidable(this);
}

bool Hero::update(NDjinn::Camera2D& cam) {
	updateBullets(cam);
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

glm::vec4 Hero::getDims() { return _xywh; }

NDjinn::CollidableType Hero::getType() { return _type; }