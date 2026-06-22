#pragma once

#include<glad/glad.h>
#include <unordered_map>
#include <iostream>
#include <string> 
#include <unordered_map>

#include "Texture.h"
#include "TextureDescriptor.h"
#include "LogTextureManager.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	// Remove all texture pointers and free memory
	void Shutdown();

	// Create white, gray, black simple textures
	void initFallback();
	Texture* load(const TextureDescriptor& texDesc);
	Texture* loadCubeMap(const TextureDescriptor& texDesc, std::array<std::string, 6> paths);
	Texture* loadRenderedTexture(const TextureDescriptor& texDesc);
	Texture* getOrLoad(const TextureDescriptor& texDesc);
	Texture* getFallback(std::string texture);

private:
	std::unordered_map<TextureDescriptor, std::unique_ptr<Texture>> texMap;	// Unqiue Pointers for more controlled creation, storage and automatic deletion

};
