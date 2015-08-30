#include "Errors.h"
#include <iostream>
#include <SDL\SDL.h>
#include <cstdlib>

void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit..." << std::endl;
	int msg;
	std::cin >> msg;
	SDL_Quit();
	exit(69);
}