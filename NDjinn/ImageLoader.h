#pragma once
#include <string>
#include "GLTexture.h"

namespace NDjinn {

	class ImageLoader {
	public:
		static GLTexture loadPng(std::string filePath);
	};

}