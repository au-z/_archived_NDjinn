#pragma once
#include <unordered_map>
#include <glm\glm.hpp>

namespace NDjinn {
	
	class InputManager
	{
	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;

	public:
		InputManager();
		~InputManager();

		void keyDown(unsigned int keyId);
		void keyUp(unsigned int keyId);

		bool isKeyDown(unsigned int keyId);

		void setMouseCoords(float x, float y);

		//getters
		glm::vec2 getMouseCoords() const { return _mouseCoords; }
	};
}

