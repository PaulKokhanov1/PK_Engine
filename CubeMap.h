#pragma once
#include <algorithm>
#include <array>

#include"Texture.h"
#include"TextureManager.h"
#include"Application.h"

class CubeMap
{
public:
	CubeMap(std::array<std::string, 6> paths);
	~CubeMap();

	// Send sub map texture unit to shader
	void sendUniformToShader(Shader& shader, const char* uniform);

	// Bind the cube map
	void Bind();
	// Unbinds cubeMap
	void Unbind();
	// Deletes cubeMap
	void Delete();

private:
	std::array<std::string, 6> cubeMapImgPaths;
	Texture* cubeMapTexture;
};
