#pragma once
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <vector>
#include "Vertex.h"

namespace NDjinn {

	enum class GlyphSort {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	struct Glyph {
		GLuint texture;
		float depth;

		Vertex tL;
		Vertex tR;
		Vertex bL;
		Vertex bR;
	};

	class RenderBatch {
	public:
		GLuint offset;
		GLuint numVerts;
		GLuint texture;
		RenderBatch(GLuint a_offset, GLuint a_numVerts, GLuint a_texture) : offset(a_offset), numVerts(a_numVerts), texture(a_texture) {}
	};

	class SpriteBatch
	{
	private:
		GLuint _vbo, _vao;
		std::vector<Glyph*> _glyphs;
		std::vector<RenderBatch> _renderBatches;
		GlyphSort _sortType;

		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSort sortType = GlyphSort::TEXTURE);
		void end();
		
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);

		void render();
	};
}
