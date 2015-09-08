#pragma once
#include "Bullet.h"
#include <glm/glm.hpp>
#include <NDjinn\AssetManager.h>
#include <NDjinn\GLTexture.h>

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float v, int life) : _pos(pos), _dir(dir), _v(v), _life(life)
{
}

Bullet::~Bullet()
{
}

void Bullet::draw(NDjinn::SpriteBatch& sprite) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static NDjinn::GLTexture tex = NDjinn::AssetManager::getTexture("assets/PNG/Enemys/Enemy_Snowman1.png");
	glm::vec4 xywh(_pos.x, _pos.y, 80, 80);
	NDjinn::Color c; c.r = 255; c.g = 255; c.b = 255, c.a = 255;
	sprite.draw(xywh, uv, tex.id, 0.0f, c); //create new bullet sprite
}

bool Bullet::update() { // true if out of life
	_pos += _dir * _v;
	_life--;
	if (_life == 0) {
		return true;
	}
	return false;
}
