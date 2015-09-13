#pragma once
#include <glm\glm.hpp>
#include <set>
#include <string>

namespace NDjinn {
	class ICollidable {
	public:
		virtual ~ICollidable() {}
		virtual void registerCollidable() = 0;
		virtual void getCollidables(glm::vec2 newPos, bool isMoving, std::set<ICollidable*>* collidables) = 0;
		virtual glm::vec4 getDims() = 0;
	};
}
