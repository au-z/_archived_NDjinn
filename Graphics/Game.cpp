#pragma once
#include <iostream>
#include <vector>

#include <NDjinn\NDjinn.h>
#include <NDjinn\Errors.h>
#include <NDjinn\Sprite.h>
#include <NDjinn\Window.h>
#include <NDjinn\AssetManager.h>
#include <NDjinn\GLTexture.h>
#include <NDjinn\QNode.h>

#include "Game.h"

Game::Game() : _time(0), _windowW(1024), _windowH(768), _gameState(GameState::PLAY)
{
	_camera.init(_windowW, _windowH, false);
}

Game::~Game()
{
}

void Game::run() {
	initSystems();
	gameLoop();
}

void Game::initSystems() {
	NDjinn::init();

	_window.create("Djinn", _windowW, _windowH, 0);
	_collider = new NDjinn::QNode(glm::vec4(-500.0f, -500.0f, 1000.0f, 1000.0f), nullptr);

	initShaders();
	_spriteBatch.init();
	_fpsLimiter.init(60.0f);
}

void Game::initShaders() {
	_shaderProgram.compileShaders("shaders/colorShader.vert", "shaders/colorShader.frag");
	_shaderProgram.addAttribute("vertexPosition");
	_shaderProgram.addAttribute("vertexColor");
	_shaderProgram.addAttribute("vertexUV");
	_shaderProgram.linkShaders();
}

void Game::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();

		processInput();
		_camera.update();

		for (unsigned int i = 0; i < _bullets.size();) {
			if (_bullets[i].update() == true) {
				_bullets[i] = _bullets.back(); // swap with back of vector
				_bullets.pop_back();
			}
			else {
				++i; // only increment if bullet is not destroyed
			}
		}

		drawGame();

		_fps = _fpsLimiter.end();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			//std::cout << "FPS: " << _fps << std::endl;
			frameCounter = 0;
		}
	}
}

void Game::processInput() {
	SDL_Event e;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.06f;

	while (SDL_PollEvent(&e) == 1) {
		//determine event type
		switch (e.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			_input.keyDown(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_input.keyUp(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_input.keyDown(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_input.keyUp(e.button.button);
			break;
		case SDL_MOUSEMOTION:
			_input.setMouseCoords(e.motion.x, e.motion.y);
			break;
		}
	}

	if (_input.isKeyDown(SDLK_w)) {
		_camera.setPos(_camera.getPos() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_input.isKeyDown(SDLK_s)) {
		_camera.setPos(_camera.getPos() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_input.isKeyDown(SDLK_a)) {
		_camera.setPos(_camera.getPos() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_input.isKeyDown(SDLK_d)) {
		_camera.setPos(_camera.getPos() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_input.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_input.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}

	if (_input.isKeyDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _camera.screenToWorldCoords(_input.getMouseCoords());
		std::cout << mouseCoords.x << "," << mouseCoords.y << std::endl;

		glm::vec2 playerPos(0.0f);
		glm::vec2 dir(mouseCoords - playerPos);
		dir = glm::normalize(dir); // create unit length vector

		_bullets.emplace_back(playerPos, dir, 10.0f, 1000);
	}
}

void Game::drawGame() {
	glClearDepth(1.0); //set depth clearing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear both buffers with bitwise OR

	_shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _shaderProgram.getUniformLocation("texSampler");
	glUniform1i(textureLocation, 0); //send textureLocation to shader

	//GLuint timeLocation = _shaderProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time); //send a variable to the shader

	//get camera matrix
	GLint pLocation = _shaderProgram.getUniformLocation("P");
	glm::mat4 camMatrix = _camera.getCamMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(camMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static NDjinn::GLTexture tex = NDjinn::AssetManager::getTexture("assets/PNG/Enemys/Enemy_Mushroom2.png");
	NDjinn::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	_spriteBatch.draw(pos, uv, tex.id, 0.0f, color);

	for (unsigned int i = 0; i < _bullets.size(); ++i) {
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();
	_spriteBatch.render();

	glBindTexture(GL_TEXTURE_2D, 0);
	_shaderProgram.unuse();
	_window.swapBuffer();
}