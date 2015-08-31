#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <string>

namespace NDjinn {

	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	private:
		int _screenW, _screenH;
		SDL_Window* _window;
		int getScreenWidth() { return _screenW; }
		int getScreenHeight() { return _screenH; }
	public:
		Window();
		~Window();

		void swapBuffer();
		int create(std::string windowName, int screenW, int screenH, unsigned int currentFlags);
	};

}
