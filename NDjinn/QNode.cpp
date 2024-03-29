#pragma once
#include <iostream>
#include <stdio.h>
#include "QNode.h"
#include "Errors.h"

namespace NDjinn {
	QNode::QNode(glm::vec4& xywh, QNode* parent) : _xywh(xywh), _children(nullptr), _parent(parent) 
	{
	}

	QNode::~QNode() {}

	bool QNode::addCollidable(ICollidable* obj) {
		if (_children == nullptr) {
			if (nodeOverflow() && MIN_RESOLUTION < _xywh.z)
			{
				grow();
				addCollidable(obj);
			}
			else {
				return(_objs.insert(obj).second);
			}
		} else {
			for (int i = 0; i < 4; ++i) {
				if (checkForOverlap(obj->getDims(), _children[i]->_xywh)) {
					_children[i]->addCollidable(obj);
				}
			}
		}
	}

	bool QNode::removeCollidable(ICollidable* obj) {
		if (_children == nullptr) {
			return static_cast<bool>(_objs.erase(obj));
		} else {
			for (int i = 0; i < 4; ++i) {
				_children[i]->removeCollidable(obj);
			}
		}
	}

	bool QNode::updateCollidable(ICollidable * obj, bool isMoving) { // default false
		//check if update is necessary
		if (isMoving) {
			removeCollidable(obj);
			addCollidable(obj);
			return true;
		}
		trim();
		return false;
	}

	void QNode::getCollidables(ICollidable* col, std::set<ICollidable*> &collidables) {
		if (_children == nullptr) {
			for (auto it : _objs) {
				if (it != col) { collidables.insert(it); }
			}
		} else {
			for (int i = 0; i < 4; ++i) {
				if (checkForOverlap(col->getDims(), _children[i]->_xywh)) {
					_children[i]->getCollidables(col, collidables);
				}
			}
		}
	}

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
			//std::cout << "Subdividing " << _xywh.x << "," << _xywh.y << " w:" << _xywh.z << " h:" << _xywh.w << std::endl;
			for (auto it : _objs) {
				addCollidable(it);
			}
			_objs.clear();
		} else {
			for (int i = 0; i < 4; i++) {
				_children[i]->grow();
			}
		}
	}

	bool QNode::trim() {
		if (_children == nullptr) return false; //can't trim leaf nodes
		// stage 1: find regions of similar resolution to combine
		int combinableCount = 0;
		for (int i = 0; i < 4; i++) {
			if (_children[i]->_children == nullptr) { combinableCount++; }
			else {
				bool trimmed = _children[i]->trim();
				if (trimmed) combinableCount++;
				else return false; // no way we can get a combinable
			}
		}
		// stage 2: detect and remove useless regions
		if (combinableCount == 4) {
			std::set<ICollidable*> combinedObjs;
			for (int i = 0; i < 4; i++) {
				for (auto it : _children[i]->_objs) {
					combinedObjs.insert(it);
				}
			}
			if (combinedObjs.size() <= MAX_COLLIDABLES_PER_LEAF) {
				_objs = combinedObjs;
				for (int i = 0; i < 4; i++) {
					delete _children[i];
				}
				delete _children;
				_children = nullptr;
				//std::cout << "Combining into " << _xywh.x << "," << _xywh.y << " w:" << _xywh.z << " h:" << _xywh.w << std::endl;
				return true; // the leaves were trimmed
			}
			else {
				return false;
			}
		}
	}

	bool QNode::nodeOverflow() {
		int collidableCount = 0;
		bool hasLazyCollidable = false;
		for (auto it : _objs) {
			// we only subdivide for strict collidables
			if (it->getType() == CollidableType::Strict) {
				collidableCount++;
			}
			else {
				hasLazyCollidable = true;
			}
		}

		//treat all nearby lazyCollidables as one collidable
		if (hasLazyCollidable) {
			collidableCount++;
		}

		return(collidableCount >= MAX_COLLIDABLES_PER_LEAF);
	}

	bool QNode::checkForOverlap(const glm::vec4& objDims, const glm::vec4& childDims) const {
		glm::vec2 objCorners[4];
		objCorners[0] = glm::vec2(objDims.x, objDims.y + objDims.w); //TL
		objCorners[1] = glm::vec2(objDims.x + objDims.z, objDims.y + objDims.w); //TR
		objCorners[2] = glm::vec2(objDims.x, objDims.y); //BL
		objCorners[3] = glm::vec2(objDims.x + objDims.z, objDims.y); //BR
		for (int i = 0; i < 4; ++i) {
			if (objCorners[i].x > childDims.x && objCorners[i].x < childDims.x + childDims.z &&
				objCorners[i].y > childDims.y && objCorners[i].y < childDims.y + childDims.w)
			{ return true; }
		}
		return false;
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