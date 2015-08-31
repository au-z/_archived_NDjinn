#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "NDjinn.h"

namespace NDjinn {

	int init() {
		SDL_Init(SDL_INIT_EVERYTHING); // init SDL with everything
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}

}