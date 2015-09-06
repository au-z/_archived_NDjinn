#pragma once
#include "Camera2D.h"
using namespace NDjinn;

Camera2D::Camera2D() : _camW(500), _camH(500), _pos(0.0f, 0.0f), _camMatrix(1.0f), _orthoMatrix(1.0f), _scale(1.0f), _needsMatrixUpdate(true)
{
}

Camera2D::~Camera2D()
{
}

void Camera2D::init(unsigned int camW, unsigned int camH) {
	_camW = camW;
	_camH = camH;

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


