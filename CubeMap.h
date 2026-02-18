#pragma once
#include <algorithm>
#include <array>

#include "Texture.h"


class CubeMap
{
public:
	CubeMap(std::array<std::string, 6> paths);
	~CubeMap();

	// Send sub map texture unit to shader
	void sendUniformToShader(Shader& shader, const char* uniform, GLuint unit);
	// Bind the cube map
	void Bind();
	// Unbinds cubeMap
	void Unbind();
	// Deletes cubeMap
	void Delete();

private:
	std::array<std::string, 6> cubeMapImgPaths;
	std::unique_ptr<Texture> cubeMapTexture;
};
