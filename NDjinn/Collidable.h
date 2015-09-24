#pragma once
#include <glm\glm.hpp>
#include "ICollidable.h"
#include "QNode.h"
using namespace NDjinn;

namespace NDjinn {
	class Collidable : public ICollidable {
	private:
		glm::vec4 _xywh;
	public:
		Collidable(glm::vec4 &xywh, NDjinn::QNode* collider);
		~Collidable();
		//setters
		void setPos(glm::vec4 & xywhChange);
		// Inherited via ICollidable
		virtual glm::vec4 getDims() override;
	};
}