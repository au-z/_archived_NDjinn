#pragma once
#include <iostream>
#include <glm\glm.hpp>
#include <unordered_map>

static enum Actor {
	Bullet,
	Hero,
	Peasant,
	ZombiePeasant,
};

class Collider
{
private:
	std::unordered_map<Actor, glm::vec4> posSpace;
public:
	Collider();
	~Collider();

	void mapSpace(Actor);
};

