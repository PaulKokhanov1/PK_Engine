#include "CubeMap.h"

CubeMap::CubeMap(std::array<std::string,6> paths)
{
	for (int i = 0; i < 6; ++i) {
		cubeMapImgPaths[i] = paths[i];
	}

	cubeMapTexture = std::make_unique<Texture>(paths, "CubeMap", GL_TEXTURE_CUBE_MAP, 0, GL_RGBA, GL_UNSIGNED_BYTE);

}

CubeMap::~CubeMap()
{
}

void CubeMap::sendUniformToShader(Shader& shader, const char* uniform, GLuint unit)
{
	cubeMapTexture->sendUniformToShader(shader, uniform, unit);
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
