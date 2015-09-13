#pragma once
#include <iostream>
#include <cmath>
#include "Camera2D.h"
using namespace NDjinn;

Camera2D::Camera2D() : _camW(500), _camH(500), _pos(0.0f, 0.0f), _camMatrix(1.0f), _orthoMatrix(1.0f), _scale(1.0f), _needsMatrixUpdate(true), _debug(false)
{
}

Camera2D::~Camera2D(){}

void Camera2D::init(unsigned int camW, unsigned int camH, bool debug) {
	_camW = camW;
	_camH = camH;
	_debug = debug;

	_orthoMatrix = glm::ortho(0.0f, (float)_camW, 0.0f, (float)_camH);
}

void Camera2D::update() {
	if (_needsMatrixUpdate) {
		//order matters (translate then scale)
		glm::vec3 translate(-_pos.x + _camW/2, -_pos.y + _camH/2, 0.0f);
		_camMatrix = glm::translate(_orthoMatrix, translate);
		
		glm::vec3 scale(_scale, _scale, 0.0f);
		_camMatrix = glm::scale(glm::mat4(1.0f), scale) * _camMatrix;
		_needsMatrixUpdate = false;

	}
}

glm::vec2 Camera2D::screenToWorldCoords(glm::vec2 screenCoords) {
	screenCoords.y = _camH - screenCoords.y; //invert
	screenCoords -= glm::vec2(_camW * 0.5f, _camH * 0.5f); // 0, 0 at center
	screenCoords /= _scale;
	screenCoords += _pos; // rely on center of camera position
	return screenCoords;
}

bool Camera2D::isInView(glm::vec2& pos) {
	int vR = ((int)_pos.x + std::abs((int)_camW / 2)) / _scale;
	int vL = ((int)_pos.x - std::abs((int)_camW / 2)) / _scale;
	int vT = ((int)_pos.y + std::abs((int)_camH / 2)) / _scale;
	int vB = ((int)_pos.y - std::abs((int)_camH / 2)) / _scale;
	if (pos.x < vR && pos.x > vL && pos.y < vT && pos.y > vB) {
		return true;
	}
	//std::cout << vL << "|" << vR << " : " << vT << "|" << vB << std::endl;
	return false;
}