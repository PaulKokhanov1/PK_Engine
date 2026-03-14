#pragma once

#include<glad/glad.h>
#include<iostream>
#include<vector>

#include"Vertex.h"
#include"Texture.h"
#include"TextureManager.h"
#include"Application.h"


class FBO
{
public:
	// Reference ID of the FrameBuffer
	GLuint ID;

	FBO(unsigned int tW, unsigned int tH, unsigned int wW, unsigned int wH);
	~FBO();

	// Generates a Vertex Buffer Object and links it to vertices
	void Construct();
	// Sends texture unit to shader
	void sendUniformToShader(Shader& shader, const char* uniform);
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
