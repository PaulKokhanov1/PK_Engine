#include "ShadowMap.h"

ShadowMap::ShadowMap(unsigned int tW, unsigned int tH) : mTextureWidth(tW), mTextureHeight(tH)
{
}

// Passing window dimensions to later remove them from needed FBO management
void ShadowMap::createDepthMapFBO(unsigned int windowWidth, unsigned int windowHeight)
{
	type = ShadowMapType::DEPTH_TEXTURE;
	FBODescriptor desc;

	// Create Depth Texutre
	TextureDescriptor depthTextureDesc;
	depthTextureDesc.width = mTextureWidth;
	depthTextureDesc.height = mTextureHeight;
	depthTextureDesc.format = GL_DEPTH_COMPONENT;
	depthTextureDesc.internalFormat = GL_DEPTH_COMPONENT24;
	depthTextureDesc.pixelType = GL_FLOAT;
	depthTextureDesc.target = GL_TEXTURE_2D;

	// Create Tex Parameters list
	depthTextureDesc.parameters =
	{
		{GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE},
		{GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL},
		{GL_TEXTURE_MIN_FILTER, GL_LINEAR},
		{GL_TEXTURE_MAG_FILTER, GL_LINEAR},
		{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER},
		{GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER}

	};

	desc.DepthTextureDesc = depthTextureDesc;
	desc.DepthRenderbufferDesc = std::nullopt;
	desc.ColorAttachmentDesc = std::nullopt;

	ShadowFBO = std::make_unique<FBO>(mTextureWidth, mTextureHeight, windowWidth, windowHeight); // use higher resolution texture width and height to allow for better magnification
	ShadowFBO->Construct(desc);

}

void ShadowMap::createDepthCubeMapFBO(unsigned int windowWidth, unsigned int windowHeight)
{
	type = ShadowMapType::DEPTH_CUBEMAP;
	FBODescriptor desc;

	// Create Depth Texture
	TextureDescriptor depthTextureDesc;
	depthTextureDesc.width = mTextureWidth;
	depthTextureDesc.height = mTextureHeight;
	depthTextureDesc.format = GL_DEPTH_COMPONENT;
	depthTextureDesc.internalFormat = GL_DEPTH_COMPONENT24;
	depthTextureDesc.pixelType = GL_FLOAT;
	depthTextureDesc.target = GL_TEXTURE_CUBE_MAP;

	// Create Tex Parameters list
	depthTextureDesc.parameters =
	{
		{GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE},
		{GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL},
		{GL_TEXTURE_MIN_FILTER, GL_NEAREST},
		{GL_TEXTURE_MAG_FILTER, GL_NEAREST},
		{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
		{GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
		{GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}

	};

	desc.DepthTextureDesc = depthTextureDesc;
	desc.DepthRenderbufferDesc = std::nullopt;
	desc.ColorAttachmentDesc = std::nullopt;

	ShadowFBO = std::make_unique<FBO>(mTextureWidth, mTextureHeight, windowWidth, windowHeight); // use higher resolution texture width and height to allow for better magnification
	ShadowFBO->Construct(desc);
}

void ShadowMap::attachFace(int faceIndex)
{
	if (type != ShadowMapType::DEPTH_CUBEMAP) {
		std::cerr << "[SHADOWMAP] Cant Attach face for non-cubemap texture" << std::endl;
		return;
	}

	if (faceIndex < 0 || faceIndex > 5) {
		std::cerr << "[SHADOWMAP] faceIndex MUST be between 0 and 5" << std::endl;
		return;
	}

	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex,
		ShadowFBO->getDepthTexture()->getID(),
		0);
}

void ShadowMap::sendShadowData(Shader& shader)
{
	switch (type) {
		case ShadowMapType::DEPTH_TEXTURE:
			shader.setSampler("shadowMap", textureSlots::RENDER_TEXTURE);	// Telling shader when looking at "shadow" use tex unit RENDER_TEXTURE
			ShadowFBO->getDepthTexture()->Bind(textureSlots::RENDER_TEXTURE);	// tell shader which texture object now sits in tex unit RENDER_TEXTURE

		break;

		case ShadowMapType::DEPTH_CUBEMAP:
			shader.setSampler("shadowCubeMap", textureSlots::RENDER_TEXTURE);
			ShadowFBO->getDepthTexture()->Bind(textureSlots::RENDER_TEXTURE);
			shader.setUniform1f("shadowCubemapResolution", std::max(mTextureWidth, mTextureHeight));
		break;

	}

}


