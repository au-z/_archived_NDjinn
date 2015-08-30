#pragma once
#include <string>
#include "GLTexture.h"

class ImageLoader {
public:
	static GLTexture loadPng(std::string filePath);
};