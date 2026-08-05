#include "FBO.h"

FBO::FBO(unsigned int tW, unsigned int tH, unsigned int wW, unsigned int wH): textureHeight(tH), textureWidth(tW), windowHeight(wH), windowWidth(wW)
{
	ID = 0;
}

FBO::~FBO()
{
	Delete();
}

void FBO::Construct(const FBODescriptor &desc)
{
	TextureManager* texManager = Application::Get().getTextureManager();

	// Grab Previous FrameBuffer 
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &origFB);

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	if (desc.ColorAttachmentDesc) {
		colorTexture = texManager->loadRenderedTexture(*desc.ColorAttachmentDesc);
	} 
	
	if (desc.DepthTextureDesc) {
		depthTexture = texManager->loadRenderedTexture(*desc.DepthTextureDesc);
	}

	if (desc.DepthRenderbufferDesc) {
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(desc.DepthRenderbufferDesc->target, depthBuffer);
		glRenderbufferStorage(desc.DepthRenderbufferDesc->target, desc.DepthRenderbufferDesc->internalFormat, desc.DepthRenderbufferDesc->width, desc.DepthRenderbufferDesc->height);
	}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[OpenGL] Error creating FBO (code " << error << ")\n";
	}

	if (desc.ColorAttachmentDesc && desc.DepthRenderbufferDesc) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture->getID(), 0);
		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}
	
	if (desc.DepthTextureDesc) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getID(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}


	if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "[OpenGL] Error FBO IS NOT READY TO USE \n";
	}

	// Unbind to orginal framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, origFB);

}

void FBO::Bind()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &origFB);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glViewport(0, 0, textureWidth, textureHeight);
}

void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, origFB);
	glViewport(0, 0, windowWidth, windowHeight);

}

void FBO::Delete()
{
	if (depthBuffer) glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteFramebuffers(1, &ID);
}

Texture* FBO::getColorTexture()
{
	return colorTexture;
}

Texture* FBO::getDepthTexture()
{
	return depthTexture;
}


