#pragma once
#include <iostream>
#include "QNode.h"
#include "Errors.h"

namespace NDjinn {
	QNode::QNode(glm::vec4& xywh, QNode* parent) : _xywh(xywh), _children(nullptr), _parent(parent) 
	{
	}

	QNode::~QNode() {}

	void QNode::grow() {
		if (_children == nullptr) {
			_children = (QNode**)malloc(4 * sizeof(QNode*));
			glm::vec4 tL(_xywh.x, _xywh.y + _xywh.w * 0.5f, _xywh.z * 0.5f, _xywh.w * 0.5f);
			glm::vec4 tR(_xywh.x + _xywh.z * 0.5f, _xywh.y + _xywh.w * 0.5f, _xywh.z * 0.5f, _xywh.w * 0.5f);
			glm::vec4 bL(_xywh.x, _xywh.y, _xywh.z * 0.5f, _xywh.w * 0.5f);
			glm::vec4 bR(_xywh.x + _xywh.z * 0.5f, _xywh.y, _xywh.z * 0.5f, _xywh.w * 0.5f);
			_children[0] = new QNode(tL, this);
			_children[1] = new QNode(tR, this);
			_children[2] = new QNode(bL, this);
			_children[3] = new QNode(bR, this);
		}
		else {
			for (int i = 0; i < 4; i++) {
				_children[i]->grow();
			}
		}
	}

	int QNode::trim() {
		if (_children == nullptr) return 1; //can't trim leaf nodes
		// stage 1: find regions of similar resolution to combine
		int combinableCount = 0;
		for (int i = 0; i < 4; i++) {
			if (_children[i]->_children == nullptr) { combinableCount++; }
			else {
				int trimmed = _children[i]->trim();
				if (trimmed == 0){
					combinableCount++;
				}
				else {
					return 1; // no way we can get a combinable
				}
			}
		}
		// stage 2: detect and remove useless regions
		if (combinableCount == 4) {
			std::set<ICollidable*> combinedCollidables;
			for (int i = 0; i < 4; i++) {
				combinedCollidables.insert(_children[i]->_collidables.begin(), _children[i]->_collidables.end());
			}
			if (combinedCollidables.size() <= MAX_COLLIDABLES_PER_LEAF) {
				//turn leaf parent into leaf
				_collidables = combinedCollidables;
				for (int i = 0; i < 4; i++) {
					delete _children[i];
				}
				_children = nullptr;
				return 0; // the leaves were trimmed
			}
			else {
				return 1;
			}
		}
	}

	int QNode::addCollidable(ICollidable * obj) {
		if (_children == nullptr) {
			//determine if subdivision is necessary
			if (_collidables.size() + 1 >= MAX_COLLIDABLES_PER_LEAF && _xywh.w >= MAX_DIVIDE_SIZE) {
				grow();
				//update each collidable
				CollideSet::iterator it = _collidables.begin();
				while (it != _collidables.end()) {
					updateCollidable(*it); //remove and re-add with new divided space
				}
				addCollidable(obj);
			}
			std::pair<CollideSet::iterator, bool> newInsertion;
			newInsertion = _collidables.insert(obj);
			if (!newInsertion.second) {
				return 1;
			}
			return 0;
		}
		else {
			//non-leaf, check for overlap in one of the children
			for (int i = 0; i < 4; ++i) {
				glm::vec4 childDims(_children[i]->getDims());
				if (checkForOverlap(obj->getDims(), childDims)) {
					_children[i]->addCollidable(obj);
				}
			}
		}
		std::cout << "Obj off the grid! Obj doesn't have a Social Security card!" << std::endl;
		return 1;
	}

	void QNode::getCollidables(ICollidable * obj, glm::vec2 newPos, bool inMotion, std::set<ICollidable*>* collidables) {
		static glm::vec4 newPosDims(newPos.x, newPos.y, obj->getDims().z, obj->getDims().w);
		if (_children == nullptr) { 
			// leaf node
			CollideSet::iterator it = _collidables.begin();
			while (it != _collidables.end()) {
				// add potential collidables
				if (*it != obj) {
					collidables->insert(*it);
				}
				it++;
			}
		}
		else {
			// branch node
			for (int i = 0; i < 4; ++i) {
				//check for overlap with children in current or next position
				bool currPosOverlap = _children[i]->checkForOverlap(obj->getDims(), _children[i]->getDims());
				bool newPosOverlap = _children[i]->checkForOverlap(newPosDims, _children[i]->getDims());
				bool overlap = (currPosOverlap || newPosOverlap);
				if (overlap) {
					// recur at next level
					_children[i]->getCollidables(obj, newPos, inMotion, collidables);
				}
			}
		}
	}

	int QNode::removeCollidable(ICollidable * obj) {
		QNode* targetRegion = findRegion(this, obj);
		CollideSet::iterator it = targetRegion->_collidables.begin();
		while (it != targetRegion->_collidables.end()) {
			if (*it == obj) {
				targetRegion->_collidables.erase(it);
				trim();
				return 0;
			}
			it++;
		}
		return 1;
	}
	
	int QNode::updateCollidable(ICollidable * obj) {
		int res = removeCollidable(obj);
		if (res == 1) return res;
		return addCollidable(obj);
	}

	bool QNode::checkForOverlap(glm::vec4& objDims, glm::vec4& childDims) const {
		glm::vec2 objCorners[4];
		objCorners[0] = glm::vec2(objDims.x, objDims.y + objDims.w); //TL
		objCorners[1] = glm::vec2(objDims.x + objDims.z, objDims.y + objDims.w); //TR
		objCorners[2] = glm::vec2(objDims.x, objDims.y); //BL
		objCorners[3] = glm::vec2(objDims.x + objDims.z, objDims.y); //BR
		for (int i = 0; i < 4; ++i) {
			if (objCorners[i].x > childDims.x &&
				objCorners[i].x < childDims.x + childDims.z &&
				objCorners[i].y > childDims.y &&
				objCorners[i].y < childDims.y + childDims.w)
			{
				return true; //yes, one of the corners is within the childDims region
			}
		}
		return false;
	}

	QNode* QNode::findRegion(QNode* startNode, ICollidable * obj) {
		if (_children == nullptr) {
			CollideSet::iterator collider = _collidables.begin();
			while (collider != _collidables.end()) {
				if (*collider == obj) {
					return this;
				}
				collider++;
			}
			return nullptr;
		}
		else {
			for (int i = 0; i < 4; ++i) {
				findRegion(_children[i], obj);
			}
		}
	}

	QNode* QNode::down(int childIndex) {
		if (_children == nullptr) {
			return nullptr;
		}
		return _children[childIndex];
	}

	QNode* QNode::up() {
		if (_parent == nullptr) {
			return nullptr;
		}
		return _parent;
	}
}