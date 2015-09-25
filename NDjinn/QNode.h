#pragma once
#include <set>
#include <glm/glm.hpp>
#include <SDL\SDL.h>
#include "Camera2D.h"
#include "ICollidable.h"

namespace NDjinn {
	class QNode {
	private:
		const int MAX_COLLIDABLES_PER_LEAF = 5;
		const int MIN_RESOLUTION = 20;
		
		const glm::vec4 _xywh;
		QNode * _parent;
		QNode ** _children;

		typedef std::set<ICollidable*> CollideSet;
		CollideSet _objs;

		void grow();
		bool nodeOverflow();
		bool checkForOverlap(const glm::vec4& objDims, const glm::vec4& childDims) const;
	public:
		QNode(glm::vec4& xywh, QNode* parent);
		~QNode();

		bool addCollidable(ICollidable* obj);
		bool removeCollidable(ICollidable* obj);
		void getCollidables(ICollidable* obj, std::set<ICollidable*> &collidables);
		// update presupposes obj has a new position
		bool updateCollidable(ICollidable* obj, bool isMoving = false);
		bool trim(); // T = trimming happened
		//void getCollidables(ICollidable* obj, glm::vec2 newPos, bool inMotion, std::set<ICollidable*>* collidables);

		//traverse
		QNode* down(int childIndex);
		QNode* up();
	};
}

