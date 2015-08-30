#pragma once
#include <vector>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <NDjinn\GLSLProgram.h>
#include <NDjinn\Sprite.h>
#include <NDjinn\GLTexture.h>

enum class GameState { PLAY, EXIT };

class Game
{
private:
	SDL_Window* _window;
	int _windowW;
	int _windowH;
	float _fps;
	Uint32 _maxFps;
	Uint32 _frameTime;

	GameState _gameState;

	void initSystems();
	void initShaders();
	void processInput();
	void gameLoop();
	void drawGame();
	void calcFPS();

	std::vector<Sprite*> _sprites;
	GLSLProgram _shaderProgram;

	float _time;

public:
	Game();
	~Game();

	void run();

};

