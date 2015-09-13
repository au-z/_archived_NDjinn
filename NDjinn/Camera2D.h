#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NDjinn {

	class Camera2D
	{
	private:
		unsigned int _camW, _camH;
		bool _debug;

		glm::vec2 _pos;
		glm::mat4 _orthoMatrix;
		glm::mat4 _camMatrix;
		float _scale;
		bool _needsMatrixUpdate;

	public:
		Camera2D();
		~Camera2D();

		void init(unsigned int camW, unsigned int camH, bool debug);
		void update();

		glm::vec2 screenToWorldCoords(glm::vec2 screenCoords);
		bool isInView(glm::vec2& pos);

		//setters
		void setPos(glm::vec2& newPos) { _pos = newPos; _needsMatrixUpdate = true; }
		void setScale(float newScale) { _scale = newScale; _needsMatrixUpdate = true; }

		//getters
		glm::vec2 getPos() { return _pos; }
		float getScale() { return _scale; }
		glm::mat4 getCamMatrix() { return _camMatrix; }
	};
}

