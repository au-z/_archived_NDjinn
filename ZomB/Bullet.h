#pragma once
#include <glm/glm.hpp>
#include <NDjinn\SpriteBatch.h>
#include <NDjinn\Camera2D.h>

class Bullet
{
private:
	glm::vec2 _pos;
	float _v;
	glm::vec2 _dir;
	int _life;

public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float v, int life);
	~Bullet();

	void draw(NDjinn::SpriteBatch& sprite);
	bool update(NDjinn::Camera2D& cam); // true if out of life
};

