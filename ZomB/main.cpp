#include "ZomB.h"
#include <set>
#include <glm\glm.hpp>
#include <NDjinn\QNode.h>
#include <NDjinn\Errors.h>
#include <NDjinn\ICollidable.h>
#include <NDjinn\Collidable.h>

#include <iostream>

int main(int argc, char ** argv) {
	glm::vec4 bounds(-100.0f, -100.0f, 200.0f, 200.0f);
	NDjinn::QNode q(bounds , nullptr);

	std::set<NDjinn::ICollidable*> c;
	
	NDjinn::Collidable c1(glm::vec4(0.0f, 0.0f, 8.0f, 8.0f), &q);
	q.addCollidable(&c1);

	NDjinn::Collidable c2(glm::vec4(80.0f, -20.0f, 8.0f, 8.0f), &q);
	q.addCollidable(&c2);

	NDjinn::Collidable c3(glm::vec4(70.0f, -30.0f, 8.0f, 8.0f), &q);
	q.addCollidable(&c3);

	q.removeCollidable(&c3);
	q.trim();

	q.getCollidables(&c2, c);
	c.clear();

	//ZomB zombieGame;
	//zombieGame.run();

	int x;
	std::cin >> x;
	return 0;
}