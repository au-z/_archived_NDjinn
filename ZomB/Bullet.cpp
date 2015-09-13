#pragma once
#include "Bullet.h"
#include <iostream>
#include <glm/glm.hpp>
#include <NDjinn\AssetManager.h>
#include <NDjinn\GLTexture.h>
#include <NDjinn\Window.h>

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float v, int life) : _pos(pos), _dir(dir), _v(v), _life(life)
{
}

Bullet::~Bullet()
{
}

void Bullet::draw(NDjinn::SpriteBatch& sprite) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static NDjinn::GLTexture tex = NDjinn::AssetManager::getTexture("assets/hero/kamehameha.png");
	glm::vec4 xywh(_pos.x, _pos.y, 30, 30);
	NDjinn::Color c; c.r = 255; c.g = 255; c.b = 255, c.a = 255;
	sprite.draw(xywh, uv, tex.id, 0.0f, c); //create new bullet sprite
}

bool Bullet::update(NDjinn::Camera2D& cam) {
	_pos += _dir * _v;
	return !cam.isInView(_pos);
}
