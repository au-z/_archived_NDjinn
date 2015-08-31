#pragma once
#include <map>
#include "GLTexture.h"

namespace NDjinn {

	class TextureCache
	{
	private:
		std::map<std::string, GLTexture> _textureMap;
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);
	};

}
