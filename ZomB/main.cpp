#include "ZomB.h"
#include <glm\glm.hpp>
#include <NDjinn\QNode.h>
#include <NDjinn\Errors.h>

#include <iostream>

int main(int argc, char ** argv) {
	ZomB zombieGame;
	zombieGame.run();

	int x;
	std::cin >> x;
	return 0;
}