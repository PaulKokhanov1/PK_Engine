#pragma once

#include<glad/glad.h>
#include<iostream>
#include<vector>

#include"Vertex.h"
#include"Texture.h"
#include"TextureManager.h"
#include"FBODescriptor.h"
#include"Application.h"


class FBO
{
public:
	// Reference ID of the FrameBuffer
	GLuint ID = 0;

	FBO(unsigned int tW, unsigned int tH, unsigned int wW, unsigned int wH);
	~FBO();

	// Generates a Vertex Buffer Object and links it to vertices
	void Construct(const FBODescriptor& desc);
	// Binds the VBO
	void Bind();
	// Unbinds the VBOz
	void Unbind();
	// Deletes the VBO
	void Delete();

	// Getters and Setters
	Texture* getColorTexture();
	Texture* getDepthTexture();

private:

	Texture* colorTexture = nullptr;
	Texture* depthTexture = nullptr;
	GLint origFB;
	GLuint depthBuffer = 0;
	unsigned int textureWidth;
	unsigned int textureHeight;
	unsigned int windowWidth;
	unsigned int windowHeight;

};
