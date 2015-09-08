#include "SpriteBatch.h"
#include <algorithm>

using namespace NDjinn;

SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
{
}


SpriteBatch::~SpriteBatch()
{
}


void SpriteBatch::init() {
	createVertexArray();
}

void SpriteBatch::createRenderBatches() {
	if (_glyphs.empty()) {
		return;
	}

	std::vector<Vertex> verts;
	verts.resize(_glyphs.size() * 6); //for memory efficiency (no multiple allocation)

	int vertOffset = 0;
	int cv = 0; // current vertex

	_renderBatches.emplace_back(0, 6, _glyphs[0]->texture);
	verts[cv++] = _glyphs[0]->tL;
	verts[cv++] = _glyphs[0]->bL;
	verts[cv++] = _glyphs[0]->bR;
	verts[cv++] = _glyphs[0]->bR;
	verts[cv++] = _glyphs[0]->tR;
	verts[cv++] = _glyphs[0]->tL;

	vertOffset += 6;

	// cg = current glyph
	for (unsigned int cg = 1; cg < _glyphs.size(); ++cg) {
		//only emplace if we're using a different texture
		if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture) {
			_renderBatches.emplace_back(vertOffset, 6, _glyphs[cg]->texture);
		}
		else {
			_renderBatches.back().numVerts += 6;
		}
		verts[cv++] = _glyphs[cg]->tL;
		verts[cv++] = _glyphs[cg]->bL;
		verts[cv++] = _glyphs[cg]->bR;
		verts[cv++] = _glyphs[cg]->bR;
		verts[cv++] = _glyphs[cg]->tR;
		verts[cv++] = _glyphs[cg]->tL;
		vertOffset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo); 
	//orphan the buffer with nullptr (for a small performance increase)
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verts.size() * sizeof(Vertex), verts.data()); //add the verts to the array buffer

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void SpriteBatch::createVertexArray() {
	if (_vao == 0) {
		glGenVertexArrays(1, &_vao);
	}

	glBindVertexArray(_vao);

	if (_vbo == 0) {
		glGenBuffers(1, &_vbo);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glEnableVertexAttribArray(0); //send an array of positions
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	//color attribute
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0); //unbind vao
}


void SpriteBatch::begin(GlyphSort sortType /* GlyphSort::TEXTURE */){
	_sortType = sortType;
	_renderBatches.clear(); //dont need to keep all vectors in memory. Prevents overflow
	
	//clear glyphs from memory
	for (int i = 0; i < _glyphs.size(); ++i) {
		delete _glyphs[i];
	}

	_glyphs.clear();
}

void SpriteBatch::end() {
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color) {
	Glyph* g = new Glyph();
	g->texture = texture;
	g->depth = depth;

	// z is width, w is height
	g->tL.color = color;
	g->tL.setPosition(destRect.x, destRect.y + destRect.w);
	g->tL.setUV(uvRect.x, uvRect.y + uvRect.w);

	g->tR.color = color;
	g->tR.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	g->tR.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	g->bL.color = color;
	g->bL.setPosition(destRect.x, destRect.y);
	g->bL.setUV(uvRect.x, uvRect.y);

	g->bR.color = color;
	g->bR.setPosition(destRect.x + destRect.z, destRect.y);
	g->bR.setUV(uvRect.x + uvRect.z, uvRect.y);

	_glyphs.push_back(g); //pass pointer to sort rather than glyph
}

void SpriteBatch::render() {
	glBindVertexArray(_vao);
	for (unsigned int i = 0; i < _renderBatches.size(); ++i) {
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVerts);
	}
}

void SpriteBatch::sortGlyphs() {
	switch (_sortType) {
	case GlyphSort::BACK_TO_FRONT:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront); //pass in predicate for third argument
		break;
	case GlyphSort::FRONT_TO_BACK:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack); //pass in predicate for third argument
		break;
	case GlyphSort::TEXTURE:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture); //pass in predicate for third argument
		break;
	}
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
	return(a->depth < b->depth);
}
bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
	return(a->depth > b->depth);
}

bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
	return(a->texture < b->texture);
}