#pragma once

#include<glad/glad.h>
#include<iostream>
#include<vector>

#include"Vertex.h"
#include"Texture.h"


class FBO
{
public:
	// Reference ID of the FrameBuffer
	GLuint ID;

	FBO(unsigned int tH, unsigned int tW, unsigned int wH, unsigned int wW);
	~FBO();

	// Generates a Vertex Buffer Object and links it to vertices
	void Construct();
	// Binds the VBO
	void Bind();
	// Unbinds the VBOz
	void Unbind();
	// Deletes the VBO
	void Delete();

	// Getters and Setters
	Texture* getRenderedTexture();

private:

	Texture* colorAttachment = nullptr;
	GLint origFB;
	GLuint depthBuffer;
	unsigned int textureWidth;
	unsigned int textureHeight;
	unsigned int windowWidth;
	unsigned int windowHeight;

};
