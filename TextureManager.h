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
	Texture* load(TextureDescriptor texDesc, const char* filename, const char* texType = "Diffuse", GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE, GLenum texTarget = GL_TEXTURE_2D);
	Texture* loadCubeMap(TextureDescriptor texDesc, std::array<std::string, 6> paths, const char* texType = "CubeMap", GLenum texTarget = GL_TEXTURE_CUBE_MAP, GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE);
	Texture* loadRenderedTexture(TextureDescriptor texDesc, unsigned int width, unsigned int height, const char* texType = "RenderTexture", GLenum texTarget = GL_TEXTURE_2D, GLenum format = GL_RGB, GLenum pixelType = GL_UNSIGNED_BYTE);
	Texture* getOrLoad(TextureDescriptor texDesc);
	Texture* getFallback(std::string texture);
	int getUnitForType(std::string);

	std::unordered_map<std::string, int> texTypeToUnit = {
		{"Fallback", 0},
		{"Diffuse", 1},
		{"Ambient", 2},
		{"Specular", 3},
		{"CubeMap", 4},
		{"RenderTexture", 5}
	};

private:
	std::unordered_map<TextureDescriptor, std::unique_ptr<Texture>> texMap;	// Unqiue Pointers for more controlled creation, storage and automatic deletion

};
