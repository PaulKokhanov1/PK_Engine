#include "FBO.h"

FBO::FBO(unsigned int tW, unsigned int tH, unsigned int wW, unsigned int wH): textureHeight(tH), textureWidth(tW), windowHeight(wH), windowWidth(wW)
{
	ID = 0;
}

FBO::~FBO()
{
	Delete();
}

void FBO::Construct()
{
	TextureManager* texManager = Application::Get().getTextureManager();

	// Grab Previous FrameBuffer 
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &origFB);

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	// Create colorAttachment Texture
	TextureDescriptor renderTextureTex;
	renderTextureTex.width = textureWidth;
	renderTextureTex.height = textureHeight;
	renderTextureTex.format = GL_RGB;
	colorAttachment = texManager->loadRenderedTexture(renderTextureTex, textureWidth, textureHeight, "RenderTexture", GL_TEXTURE_2D, GL_RGB, GL_UNSIGNED_BYTE);

	// Create Depth Buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureWidth, textureHeight);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[OpenGL] Error creating FBO (code " << error << ")\n";
	}
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorAttachment->getID(), 0);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "[OpenGL] Error FBO IS NOT READY TO USE FBO \n";
	}

	// Unbind to orginal framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, origFB);

}

void FBO::sendUniformToShader(Shader& shader, const char* uniform)
{
	TextureManager* texManager = Application::Get().getTextureManager();

	colorAttachment->sendUniformToShader(shader, uniform, texManager->texTypeToUnit[colorAttachment->getType()]);
}

void FBO::Bind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID);
	glViewport(0, 0, textureWidth, textureHeight);
}

void FBO::Unbind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, origFB);
	glViewport(0, 0, windowWidth, windowHeight);

}

void FBO::Delete()
{
	glDeleteFramebuffers(1, &ID);
}

Texture* FBO::getRenderedTexture()
{
	return colorAttachment;
}

