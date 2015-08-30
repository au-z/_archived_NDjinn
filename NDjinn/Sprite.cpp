#include <cstddef>
#include "Sprite.h"
#include "Vertex.h"
#include "AssetManager.h"

Sprite::Sprite() : _vboID(0)
{
}

Sprite::~Sprite()
{
	if (_vboID != 0) glDeleteBuffers(1, &_vboID);
}

void Sprite::init(float x, float y, float w, float h, std::string texturePath) {
	_x = x; _y = y; _w = w; _h = h;

	_tex = AssetManager::getTexture(texturePath);

	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	Vertex vertexData[6]; // x and y for 6 verts for one quad
	mapVertexData(vertexData);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW); //put in buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
}

void Sprite::mapVertexData(Vertex* vertexData) {
	// top-right
	vertexData[0].setPosition(_x + _w, _y + _h);
	vertexData[5].setPosition(_x + _w, _y + _h);
	vertexData[0].setUV(1, 1);
	vertexData[5].setUV(1, 1);
	//top-left
	vertexData[1].setPosition(_x, _y + _h);
	vertexData[1].setUV(0, 1);
	//bottom-left
	vertexData[2].setPosition(_x, _y);
	vertexData[3].setPosition(_x, _y);
	vertexData[2].setUV(0, 0);
	vertexData[3].setUV(0, 0);
	// bottom-right
	vertexData[4].setPosition(_x + _w, _y);
	vertexData[4].setUV(1, 0);

	for (int i = 0; i < 6; ++i) {
		vertexData[i].setColor(255, 255, 0, 255);
	}
}

void Sprite::draw() {

	glBindTexture(GL_TEXTURE_2D, _tex.id); // do not unbind
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glEnableVertexAttribArray(0); //send an array of positions
	//position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	//color attribute
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
}
