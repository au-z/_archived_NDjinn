#pragma once
#include <GL/glew.h>
#include "Vertex.h"
#include "GLTexture.h"
#include <string>

class Sprite
{
private:
	float _x, _y, _w, _h;
	GLuint _vboID; 
	GLTexture _tex;

	void mapVertexData(Vertex* vertexData);
public:
	Sprite();
	~Sprite();
	
	void init(float x, float y, float w, float h, std::string texturePath);
	void draw();
};