#pragma once
#include <iostream>
#include <vector>
#include <NDjinn\Errors.h>
#include <NDjinn\Sprite.h>

#include "Game.h"

Game::Game() : _time(0), _window(nullptr), _windowW(1024), _windowH(768), _gameState(GameState::PLAY), _maxFps(60)
{

}

Game::~Game()
{

}

void Game::run() {
	initSystems();
	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "assets/PNG/Enemys/Enemy_Broccoli1.png");
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, 0.0f, 1.0f, 1.0f, "assets/PNG/Enemys/Enemy_Snowman1.png");
	gameLoop();
}

void Game::initSystems() {
	SDL_Init(SDL_INIT_EVERYTHING); // init SDL with everything
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	_window = SDL_CreateWindow("Game Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,_windowW,_windowH,SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		fatalError("SDL window could not be created.");
	}

	//openGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could not be created.");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) { // 0
		fatalError("Could not initialize GLEW.");
	}

	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));
	SDL_GL_SetSwapInterval(1); // V-SYNC

	//bg color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	initShaders();
}

void Game::initShaders() {
	_shaderProgram.compileShaders("shaders/colorShader.vert", "shaders/colorShader.frag");
	_shaderProgram.addAttribute("vertexPosition");
	_shaderProgram.addAttribute("vertexColor");
	_shaderProgram.addAttribute("vertexUV");
	_shaderProgram.linkShaders();
}

void Game::processInput() {
	SDL_Event e;
	
	while (SDL_PollEvent(&e) == 1) {
		//determine event type
		switch (e.type) {
		case SDL_QUIT: 
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << e.motion.x << "," << e.motion.y << std::endl;
			break;
		}
	}
}

void Game::gameLoop() {
	while (_gameState != GameState::EXIT) {
		Uint32 loopStart = SDL_GetTicks();

		processInput();
		_time += 0.01f; //change to FPS

		calcFPS();
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			std::cout << "FPS: " << _fps << std::endl;
			frameCounter = 0;
		}

		drawGame();

		//FPS limiter
		Uint32 loopEnd = SDL_GetTicks();
		Uint32 elapsedTime = loopEnd - loopStart;
		if (1000.0f / (Uint32)_maxFps > elapsedTime) {
			SDL_Delay(1000 / (Uint32)_maxFps - elapsedTime);
		}
	}
}

void Game::drawGame() {
	glClearDepth(1.0); //set depth clearing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear both buffers with bitwise OR

	_shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _shaderProgram.getUniformLocation("texSampler");
	glUniform1i(textureLocation, 0); //send textureLocation to shader

	GLuint timeLocation = _shaderProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time); //send a variable to the shader

	for (int i = 0; i < _sprites.size(); ++i) {
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	_shaderProgram.unuse();
	SDL_GL_SwapWindow(_window);
}

void Game::calcFPS() {
	static const int NUM_SAMPLES = 10;
	static Uint32 frameTimeBuffer[NUM_SAMPLES];
	static Uint32 frameNum = 0;
	static Uint32 previousTicks = 0;

	Uint32 currentTicks = SDL_GetTicks();
	_frameTime = currentTicks - previousTicks;

	frameTimeBuffer[frameNum % NUM_SAMPLES] = _frameTime;

	float frameTimeAvg = 0;
	if (frameNum > NUM_SAMPLES) {
		for (int i = 0; i < NUM_SAMPLES; i++) {
			frameTimeAvg += frameTimeBuffer[i];
		}
		frameTimeAvg /= NUM_SAMPLES;

		if (frameTimeAvg > 0) {
			_fps = 1000.0f / frameTimeAvg;
		}
	}
	else {
		_fps = 0.0f;
	}

	previousTicks = currentTicks;
	frameNum++;
}