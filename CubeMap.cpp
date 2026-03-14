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
	cubeMapTexture = texManager->loadCubeMap(cubeMapTex, paths, "CubeMap", GL_TEXTURE_CUBE_MAP, GL_RGBA, GL_UNSIGNED_BYTE);
}

CubeMap::~CubeMap()
{
}

void CubeMap::sendUniformToShader(Shader& shader, const char* uniform)
{
	TextureManager* texManager = Application::Get().getTextureManager();

	cubeMapTexture->sendUniformToShader(shader, uniform, texManager->texTypeToUnit[cubeMapTexture->getType()]);
}

void CubeMap::Bind()
{
	cubeMapTexture->Bind();
}

void CubeMap::Unbind()
{
	cubeMapTexture->Unbind();
}

void CubeMap::Delete()
{
	cubeMapTexture->Delete();
}
