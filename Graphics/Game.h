#pragma once
#include <vector>
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

#include "Bullet.h"

enum class GameState { PLAY, EXIT };

class Game
{
private:
	NDjinn::Window _window;
	int _windowW;
	int _windowH;

	GameState _gameState;

	void initSystems();
	void initShaders();
	void processInput();
	void gameLoop();
	void drawGame();
	void calcFPS();

	NDjinn::GLSLProgram _shaderProgram;
	NDjinn::Camera2D _camera;
	NDjinn::SpriteBatch _spriteBatch;
	NDjinn::InputManager _input;
	NDjinn::FpsLimiter _fpsLimiter;

	float _fps;
	float _time;

	std::vector<Bullet> _bullets;

public:
	Game();
	~Game();

	void run();

};