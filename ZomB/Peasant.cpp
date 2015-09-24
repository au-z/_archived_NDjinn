#pragma once
#include <iostream>
#include <cstdlib>
#include <NDjinn\AssetManager.h>
#include "Peasant.h"

Peasant::Peasant() :
	_xywh(-100.0f, -100.0f, 50.0f, 50.0f),
	_uv(0.0f, 0.0f, 1.0f, 1.0f),
	_speed(5.0f),
	_prevDirs(SMOOTHING_COUNT, glm::vec2(1.0))
{
}

Peasant::~Peasant(){}

void Peasant::init(NDjinn::QNode* collider, bool isZombie = false) {
	_collider = collider;
	_isZombie = isZombie;
	setTex();
	_color.r = 255; _color.g = 255; _color.b = 255; _color.a = 255;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	_collider->addCollidable(this);
}

bool Peasant::update(glm::vec2 heroPos) {
	/*static unsigned int frame = 0;

	int dirX = (rand() % 2000) - 1000;
	int dirY = (rand() % 2000) - 1000;
	glm::vec2 dir(dirX, dirY);

	for (int i = 0; i < _prevDirs.size(); ++i) {
		dir += _prevDirs[i];
	}

	dir = glm::normalize(dir);
	_xywh += glm::vec4(dir.x, dir.y, 0.0f, 0.0f) * _speed;

	_prevDirs[frame] = dir;
	if (frame == SMOOTHING_COUNT - 1) {
		frame = 0;
	}

	frame++;*/
	_collider->updateCollidable(this);
	return false;
}

void Peasant::move(glm::vec2 dir) {
	_xywh.x += dir.x;
	_xywh.y += dir.y;
}

void Peasant::draw(NDjinn::SpriteBatch& sprites) {
	sprites.draw(_xywh, _uv, _tex.id, 0.0f, _color);
}

void Peasant::setTex() {
	_tex = NDjinn::AssetManager::getTexture("assets/peasant/peasant.png");
}

glm::vec4 Peasant::getDims() { return _xywh; }