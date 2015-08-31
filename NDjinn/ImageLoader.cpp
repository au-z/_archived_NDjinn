#include "ImageLoader.h"
#include "picoPng.h"
#include "AssetManager.h"
#include "Errors.h"
using namespace NDjinn;

GLTexture ImageLoader::loadPng(std::string filePath) {
	GLTexture texture = {};

	unsigned long width, height;
	std::vector<unsigned char> out;
	std::vector<unsigned char> in;

	if (AssetManager::readFileToBuffer(in, filePath) == false) {
		perror(filePath.c_str());
		fatalError("Could not read image from file");
	}

	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
	if (errorCode != 0) {
		fatalError("Decode PNG failed with error " + errorCode);
	}

	glGenTextures(1, &(texture.id));
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //magnification filter (linear interpolation)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //minification filter (linear interpolation)

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.w = width;
	texture.h = height;

	return texture;
}