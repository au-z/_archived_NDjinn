#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <NDjinn\GLSLProgram.h>
#include <NDjinn\Sprite.h>
#include <NDjinn\GLTexture.h>
#include <NDjinn\Window.h>
#include <NDjinn\Camera2D.h>
#include <NDjinn\SpriteBatch.h>
#include <NDjinn\InputManager.h>
#include <NDjinn\Timing.h>
#include <NDjinn\QNode.h>

#include "Hero.h"
#include "Peasant.h"

enum class GameState { PLAY, EXIT };

class ZomB
{
private:
	int _winW, _winH;
	NDjinn::Window _win;
	NDjinn::Camera2D _cam;
	NDjinn::QNode* _collider;
	NDjinn::GLSLProgram _shaderProgram;
	NDjinn::SpriteBatch _sprites;
	NDjinn::InputManager _input;
	NDjinn::FpsLimiter _fpsLimiter;

	Hero _hero;
	Peasant _peasant;

	GameState _state;

	void initSystems();
	void initShaders();
	void processInput();
	void mainLoop();
	void draw();

	float _fps;
	float _time;
public:
	ZomB();
	~ZomB();

	void run();
};

