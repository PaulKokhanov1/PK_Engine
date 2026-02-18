#pragma once

#include<glad/glad.h>
#include <unordered_map>
#include <iostream>
#include <string> 

#include "Texture.h"

using namespace std;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	// Remove all texture pointers and free memory
	void Shutdown();

	// Create white, gray, black simple textures
	void initFallback();
	Texture* load(const char* filename, const char* texType = "Diffuse", GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE, GLenum texTarget = GL_TEXTURE_2D);
	Texture* getOrLoad(string texture);
	Texture* getFallback(string texture);
	


private:

	unordered_map<string, int> texTypeToUnit = {
		{"Fallback", 0},
		{"Diffuse", 1},
		{"Ambient", 2},
		{"Specular", 3},
		{"CubeMap", 4}
	};
	unordered_map<string, Texture*> texMap;

};
