#pragma once
#include <glm\glm.hpp>
#include <set>
#include <string>

namespace NDjinn {
	class ICollidable {
	public:
		virtual ~ICollidable() {}
		virtual glm::vec4 getDims() = 0;
	};
}
