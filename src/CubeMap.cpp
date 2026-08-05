#include "CubeMap.h"

CubeMap::CubeMap(std::array<std::string,6> paths)
{
	for (int i = 0; i < 6; ++i) {
		cubeMapImgPaths[i] = paths[i];
	}

	// Each material holds 1 type of texture
	TextureManager* texManager = Application::Get().getTextureManager();

	// prior to load cubemap does not know with and height, so store as 0
	TextureDescriptor cubeMapTex;
	cubeMapTex.path = paths[0] + paths[1] + paths[2] + paths[3] + paths[4] + paths[5];
	cubeMapTex.format = GL_RGBA;
	cubeMapTex.target = GL_TEXTURE_CUBE_MAP;
	cubeMapTex.pixelType = GL_UNSIGNED_BYTE;

	cubeMapTexture = texManager->loadCubeMap(cubeMapTex, paths);
}

CubeMap::~CubeMap()
{
}

void CubeMap::Bind(uint32_t slot)
{
	cubeMapTexture->Bind(slot);
}

void CubeMap::Unbind()
{
	cubeMapTexture->Unbind();
}

void CubeMap::Delete()
{
	cubeMapTexture->Delete();
}
