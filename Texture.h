#pragma once

#include<glad/glad.h>

#include"ShaderManager.h"
#include"lodepng.h"
#include"LogTexture.h"

class Texture
{
public:
	Texture(const char* filename, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	~Texture();


	// Sends texture unit to shader
	void sendUniformToShader(Shader& shader, const char* uniform);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

private:

	// Number refering to texture
	GLuint texID;
	// Texture type;
	GLenum type;
	// Texture unit
	GLuint unit;


};
