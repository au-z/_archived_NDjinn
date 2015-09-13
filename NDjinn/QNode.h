#pragma once
#include <set>
#include <glm/glm.hpp>
#include <SDL\SDL.h>
#include "Camera2D.h"
#include "ICollidable.h"

namespace NDjinn {
	class QNode {
	private:
		const int MAX_COLLIDABLES_PER_LEAF = 1;
		const int MAX_DIVIDE_SIZE = 10;
		
		glm::vec4 _xywh;
		QNode * _parent;
		QNode ** _children;

		typedef std::set<ICollidable*> CollideSet;
		CollideSet _collidables;

		bool checkForOverlap(glm::vec4& objDims, glm::vec4& childDims) const;
		QNode* findRegion(QNode* startNode, ICollidable * obj);
	public:
		QNode(glm::vec4& xywh, QNode* parent);
		~QNode();

		void subDiv();
		int addCollidable(ICollidable* obj);
		void getCollidables(ICollidable* obj, glm::vec2 newPos, bool inMotion, std::set<ICollidable*>* collidables);
		int removeCollidable(QNode * startNode, ICollidable * obj);
		// update presupposes obj has a new position
		int updateCollidable(QNode * startNode, ICollidable * obj);

		//traverse
		QNode* down(int childIndex);
		QNode* up();
		
		//getters
		glm::vec4 getDims() { return _xywh; }
		bool isLeaf() { return (_children == nullptr); }
	};
}

