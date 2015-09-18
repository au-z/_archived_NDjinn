#pragma once
#include <glm/glm.hpp>
#include <NDjinn\SpriteBatch.h>
#include <NDjinn\Camera2D.h>
#include <NDjinn\ICollidable.h>
#include <NDjinn\QNode.h>

class Bullet : public NDjinn::ICollidable{
private:
	glm::vec4 _xywh;
	float _v;
	glm::vec2 _dir;

	NDjinn::QNode* _collider;
	std::set<ICollidable*> _collidables;

public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float v, NDjinn::QNode* collider);
	~Bullet();

	void init(NDjinn::QNode* collider);
	void draw(NDjinn::SpriteBatch& sprite);
	bool update(NDjinn::Camera2D& cam);
	// Inherited via ICollidable
	virtual void registerCollidable() override;
	virtual void getCollidables(glm::vec2 newPos, bool isMoving, std::set<ICollidable*>* collidables) override;
	virtual glm::vec4 getDims() override;
	// true if out of life
};

