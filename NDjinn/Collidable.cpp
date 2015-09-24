#pragma once
#include "Collidable.h"

using namespace NDjinn;

Collidable::Collidable(glm::vec4 &xywh, NDjinn::QNode* collider) : _xywh(xywh) {}
Collidable::~Collidable() {}
//setters
void Collidable::setPos(glm::vec4 & xywhChange) { _xywh += xywhChange; }
// Inherited via ICollidable
glm::vec4 Collidable::getDims() { return _xywh; }