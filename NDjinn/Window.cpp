#include "Window.h"
#include "Errors.h"
using namespace NDjinn;

Window::Window()
{
}

Window::~Window()
{
}

int Window::create(std::string windowName, int screenW, int screenH, unsigned int currentFlags) {

	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE) {
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (currentFlags & BORDERLESS) {
		flags |= SDL_WINDOW_BORDERLESS;
	}

	_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, flags);
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//bg color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return 0;
}

void Window::swapBuffer() {
	SDL_GL_SwapWindow(_window);
}
