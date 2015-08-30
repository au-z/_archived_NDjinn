#pragma once
#include <iostream>
#include "TextureCache.h"
#include "ImageLoader.h"
#include "GLTexture.h"

TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}

GLTexture TextureCache::getTexture(std::string texturePath) {
	auto mapIterator = _textureMap.find(texturePath); //get ptr to path if it exists
	if (mapIterator == _textureMap.end()) {
		//load and insert into the map
		GLTexture texture = ImageLoader::loadPng(texturePath);
		_textureMap.insert(make_pair(texturePath, texture));
		std::cout << "New texture loaded. \n";
		return texture;
	}
	
	std::cout << "Cached texture found...applying. \n";
	return mapIterator->second; // return the value from our iterator


}
