#include "AssetManager.h"
#include <fstream>

TextureCache AssetManager::_textureCache;

bool AssetManager::readFileToBuffer(std::vector<unsigned char>& buffer, std::string filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	//seek to the end of the file
	file.seekg(0, std::ios::end);
	//get the file size
	int fileSize = (int)file.tellg();
	//seek to the beginning for reading
	file.seekg(0, std::ios::beg);
	// remove header bytes
	fileSize -= (int)file.tellg();

	buffer.resize(fileSize);
	file.read((char *)&(buffer[0]), fileSize);
	file.close();
	return true;
}

GLTexture AssetManager::getTexture(std::string texturePath) {
	return _textureCache.getTexture(texturePath);
}




