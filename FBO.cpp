#include "FBO.h"

FBO::FBO(unsigned int tH, unsigned int tW, unsigned int wH, unsigned int wW): textureHeight(tH), textureWidth(tW), windowHeight(wH), windowWidth(wW)
{
	ID = 0;
}

FBO::~FBO()
{
	Delete();
}

void FBO::Construct()
{
	// Grab Previous FrameBuffer 
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &origFB);

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	// Create colorAttachment Texture
	colorAttachment = new Texture(GL_RGB, GL_UNSIGNED_BYTE, textureWidth, textureHeight);

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

