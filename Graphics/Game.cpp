#pragma once
#include <iostream>
#include <vector>

#include <NDjinn\NDjinn.h>
#include <NDjinn\Errors.h>
#include <NDjinn\Sprite.h>
#include <NDjinn\Window.h>

#include "Game.h"

Game::Game() : _time(0), _windowW(1024), _windowH(768), _gameState(GameState::PLAY), _maxFps(60)
{
}

Game::~Game()
{
}

void Game::run() {
	initSystems();
	_sprites.push_back(new NDjinn::Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "assets/PNG/Enemys/Enemy_Broccoli1.png");
	_sprites.push_back(new NDjinn::Sprite());
	_sprites.back()->init(0.0f, 0.0f, 1.0f, 1.0f, "assets/PNG/Enemys/Enemy_Snowman1.png");
	gameLoop();
}

void Game::initSystems() {
	NDjinn::init();

	_window.create("Djinn", _windowW, _windowH, 0);

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

	for (unsigned int i = 0; i < _sprites.size(); ++i) {
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	_shaderProgram.unuse();
	_window.swapBuffer();
}

void Game::calcFPS() {
	static const int NUM_SAMPLES = 10;
	static Uint32 frameTimeBuffer[NUM_SAMPLES];
	static Uint32 frameNum = 0;
	static Uint32 previousTicks = 0;

	Uint32 currentTicks = SDL_GetTicks();
	_frameTime = currentTicks - previousTicks;

	frameTimeBuffer[frameNum % NUM_SAMPLES] = _frameTime;

	float frameTimeAvg = 0.0f;
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