#pragma once
#include <glm/glm.hpp>
#include <NDjinn\AssetManager.h>
#include <NDjinn\SpriteBatch.h>
#include <NDjinn\Camera2D.h>
#include <NDjinn\ICollidable.h>
#include <NDjinn\QNode.h>
#include <NDjinn\GLTexture.h>

class Bullet : public NDjinn::ICollidable{
private:
	NDjinn::CollidableType _type;
	glm::vec4 _xywh;
	NDjinn::GLTexture _tex;
	glm::vec4 _uv;
	NDjinn::Color _color;

	float _v;
	glm::vec2 _dir;

	NDjinn::QNode* _collider;
	std::set<ICollidable*> _collidables;

public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float v);
	~Bullet();

	void init(NDjinn::QNode* collider);
	bool update(NDjinn::Camera2D& cam);
	void draw(NDjinn::SpriteBatch& sprite);
	// Inherited via ICollidable
	virtual glm::vec4 getDims() override;
	virtual NDjinn::CollidableType getType() override;
};
