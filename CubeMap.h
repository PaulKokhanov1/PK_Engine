#pragma once
#include <algorithm>
#include <array>

#include"EngineConfig.h"
#include"Texture.h"
#include"TextureManager.h"
#include"Application.h"

class CubeMap
{
public:
	CubeMap(std::array<std::string, 6> paths);
	~CubeMap();

	// Bind the cube map
	void Bind(uint32_t slot);
	// Unbinds cubeMap
	void Unbind();
	// Deletes cubeMap
	void Delete();

private:
	std::array<std::string, 6> cubeMapImgPaths;
	Texture* cubeMapTexture;
};
