#pragma once
#include <iostream>
#include <vector>

#include <NDjinn\NDjinn.h>
#include <NDjinn\Errors.h>
#include <NDjinn\Sprite.h>
#include <NDjinn\Window.h>
#include <NDjinn\AssetManager.h>
#include <NDjinn\GLTexture.h>
#include <NDjinn\ICollidable.h>
#include "ZomB.h"

ZomB::ZomB() : _time(0), _winW(1024), _winH(768), _state(GameState::PLAY)
{
	_cam.init(_winW, _winH, false);
}

ZomB::~ZomB(){}

void ZomB::run() {
	initSystems();
	mainLoop();
}

void ZomB::initSystems() {
	NDjinn::init();
	_win.create("ZomB", _winW, _winH, 0);
	initShaders();

	_sprites.init();
	_fpsLimiter.init(60.0f);

	//TODO: replace with world coordinates
	_collider = new NDjinn::QNode(glm::vec4(-500.0f, -500.0f, 1000.0f, 1000.0f), nullptr);
	
	_hero.init(_collider);
	_peasant.init(_collider, false); //non-zombie
}

void ZomB::initShaders() {
	_shaderProgram.compileShaders("shaders/colorShader.vert", "shaders/colorShader.frag");
	_shaderProgram.addAttribute("vertexPosition");
	_shaderProgram.addAttribute("vertexColor");
	_shaderProgram.addAttribute("vertexUV");
	_shaderProgram.linkShaders();
}

void ZomB::mainLoop() {
	while (_state != GameState::EXIT) {
		_fpsLimiter.begin();

		processInput();
		_cam.update();

		_hero.update(_cam);
		//_peasant.update(_hero.getPos());
		draw();

		_fps = _fpsLimiter.end();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 80) {
			//std::cout << "FPS: " << _fps << std::endl;
			frameCounter = 0;
		}
	}
}

void ZomB::processInput() {
	SDL_Event e;

	const float CAM_SPEED = 3.0f;
	const float SCALE_SPEED = 0.08f;

	while (SDL_PollEvent(&e) == 1) {
		switch (e.type) {
		case SDL_QUIT:
			_state = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			_input.keyDown(e.key.keysym.sym);
			if (_input.isKeyDown(SDLK_SPACE)) {
				glm::vec2 mouseCoords = _cam.screenToWorldCoords(_input.getMouseCoords());
				glm::vec2 dir(mouseCoords - _hero.getPos());
				dir = glm::normalize(dir);
				_hero.shoot(dir);
			}
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
			_input.setMouseCoords((float)e.motion.x, (float)e.motion.y);
			break;
		}
	}

	//CAM
	if (_input.isKeyDown(SDLK_UP)) {
		_cam.setPos(_cam.getPos() + glm::vec2(0.0f, CAM_SPEED));
	}
	if (_input.isKeyDown(SDLK_DOWN)) {
		_cam.setPos(_cam.getPos() + glm::vec2(0.0f, -CAM_SPEED));
	}
	if (_input.isKeyDown(SDLK_LEFT)) {
		_cam.setPos(_cam.getPos() + glm::vec2(-CAM_SPEED, 0.0f));
	}
	if (_input.isKeyDown(SDLK_RIGHT)) {
		_cam.setPos(_cam.getPos() + glm::vec2(CAM_SPEED, 0.0f));
	}
	if (_input.isKeyDown(SDLK_q)) {
		_cam.setScale(_cam.getScale() + SCALE_SPEED);
	}
	if (_input.isKeyDown(SDLK_e)) {
		_cam.setScale(_cam.getScale() - SCALE_SPEED);
	}

	//HERO
	if (_input.isKeyDown(SDLK_d)) {
		_hero.move(glm::vec2(_hero.getSpeed(), 0.0f));
	}
	if (_input.isKeyDown(SDLK_a)) {
		_hero.move(glm::vec2(-_hero.getSpeed(), 0.0f));
	}
	if (_input.isKeyDown(SDLK_w)) {
		_hero.move(glm::vec2(0.0f, _hero.getSpeed()));
	}
	if (_input.isKeyDown(SDLK_s)) {
		_hero.move(glm::vec2(0.0f, -_hero.getSpeed()));
	}

	//TEST OUTPUT
	if (_input.isKeyDown(SDLK_p)) {
		
	}
}

void ZomB::draw() {
	glClearDepth(1.0); //depth clearing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint texLocation = _shaderProgram.getUniformLocation("texSampler");
	glUniform1i(texLocation, 0);

	GLint pLocation = _shaderProgram.getUniformLocation("P");
	glm::mat4 camMatrix = _cam.getCamMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(camMatrix[0][0]));

	_sprites.begin();

	_hero.draw(_sprites);
	_peasant.draw(_sprites);

	_sprites.end();
	_sprites.render();

	glBindTexture(GL_TEXTURE_2D, 0);
	_shaderProgram.unuse();
	_win.swapBuffer();
}
