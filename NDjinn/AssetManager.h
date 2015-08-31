#pragma once
#include <vector>
#include "TextureCache.h"

namespace NDjinn {

	class AssetManager
	{
	private:
		static TextureCache _textureCache;
	public:
		static bool readFileToBuffer(std::vector<unsigned char>& buffer, std::string filePath);
		static GLTexture getTexture(std::string texturePath);
	};

}