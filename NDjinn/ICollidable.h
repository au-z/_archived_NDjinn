#pragma once
#include <glm\glm.hpp>
#include <set>
#include <string>

namespace NDjinn {
	enum CollidableType { // Collidable Type
		Strict, // collides with anything
		Lazy // only collides with strict
	};

	class ICollidable {
	public:
		virtual ~ICollidable() {}
		virtual glm::vec4 getDims() = 0;
		virtual CollidableType getType() = 0;
	};
}
