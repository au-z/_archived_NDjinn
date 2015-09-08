#include "InputManager.h"
using namespace NDjinn;


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::setMouseCoords(float x, float y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}

void InputManager::keyDown(unsigned int keyId) {
	_keyMap[keyId] = true;
}

void InputManager::keyUp(unsigned int keyId) {
	_keyMap[keyId] = false;
}

bool InputManager::isKeyDown(unsigned int keyId) {
	auto it = _keyMap.find(keyId);
	if (it != _keyMap.end()) { //false if key not found in map
		return it->second;
	}
	return false;
}
