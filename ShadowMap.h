#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"FBO.h"
#include "EngineConfig.h"

enum class ShadowMapType {
	DEPTH_TEXTURE,
	DEPTH_CUBEMAP
};

class ShadowMap
{
public:
	ShadowMap(unsigned int tW, unsigned int tH);
	~ShadowMap() {};

	void createDepthMapFBO(unsigned int windowWidth, unsigned int windowHeight);
	void createDepthCubeMapFBO(unsigned int windowWidth, unsigned int windowHeight);
	void attachFace(int faceIndex);

	void sendShadowData(Shader& shader);

	FBO& getShadowFBO() { return *ShadowFBO; }
	ShadowMapType getShadowMapType() const { return type; }
	std::pair<int, int> getDepthTextureDimensions() const { return { mTextureWidth, mTextureHeight }; }

private:

	std::unique_ptr<FBO> ShadowFBO;
	unsigned int mTextureWidth;
	unsigned int mTextureHeight;
	ShadowMapType type;

};