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
			std::cout << "Subdividing " << _xywh.x << "," << _xywh.y << " w:" << _xywh.z << " h:" << _xywh.w << std::endl;
		}
		else {
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
			std::set<ICollidable*> combinedCollidables;
			for (int i = 0; i < 4; i++) {
				combinedCollidables.insert(_children[i]->_collidables.begin(), _children[i]->_collidables.end());
			}
			if (combinedCollidables.size() <= MAX_COLLIDABLES_PER_LEAF) {
				_collidables = combinedCollidables;
				for (int i = 0; i < 4; i++) {
					delete _children[i];
				}
				delete _children;
				_children = nullptr;
				std::cout << "Combining into " << _xywh.x << "," << _xywh.y << " w:" << _xywh.z << " h:" << _xywh.w << std::endl;
				return true; // the leaves were trimmed
			}
			else {
				return false;
			}
		}
	}

	bool QNode::addCollidable(ICollidable * obj) {
		if (_children == nullptr) {
			if (_collidables.size() == MAX_COLLIDABLES_PER_LEAF && _xywh.w >= MAX_DIVIDE_SIZE) {
				//leaf with more than one collidable
				grow();
				std::set<ICollidable*> collideCopy(_collidables);
				collideCopy.insert(obj);
				for (auto it : collideCopy) {
					addCollidable(it);
				}
				_collidables.clear();
				return true;
			}
			else {
				//leaf node with no collidables
				std::pair<CollideSet::iterator, bool> newInsertion;
				newInsertion = _collidables.insert(obj);
				if (newInsertion.second) { //true if inserted
					return true;
				}
				return false;
			}
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
	}

	bool QNode::removeCollidable(ICollidable * obj) {
		if (_children == nullptr) {
			int numErased = _collidables.erase(obj);
			if (numErased > 0) {
				return true;
			}
			return false;
		}
		else {
			for (int i = 0; i < 4; ++i) {
				_children[i]->removeCollidable(obj);
			}
		}
	}
	
	int QNode::updateCollidable(ICollidable * obj) {
		bool removed = removeCollidable(obj);
		bool added = addCollidable(obj);
		trim();
		if (removed && added) {
			return 1;
		}
		fatalError("Quadtree doesn't abide bureaucracy, refuses to update obj.");
		return 0;
	}

	void QNode::getCollidables(ICollidable * obj, glm::vec2 newPos, bool inMotion, std::set<ICollidable*>* collidables) {
		static glm::vec4 newPosDims(newPos.x, newPos.y, obj->getDims().z, obj->getDims().w);
		if (_children == nullptr) {
			// leaf node, add collidables to watch set
			collidables->insert(_collidables.begin(), _collidables.end());
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

	QNode* QNode::findNode(QNode* startNode, ICollidable * obj) {
		startNode->_children = nullptr;
		if (startNode->_children == nullptr) {
			auto it = _collidables.find(obj);
			if (it != _collidables.end()) {
				return this;
			}
			return nullptr;
		}
		else {
			for (int i = 0; i < 4; ++i) {
				findNode(startNode->_children[i], obj);
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