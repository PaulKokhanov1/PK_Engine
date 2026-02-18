#pragma once

#include<glad/glad.h>
#include <array>

#include"ShaderManager.h"
#include"lodepng.h"
#include"LogTexture.h"

class Texture
{
public:
	Texture(const char* filename, const char* texType, GLenum slot, GLenum format, GLenum pixelType, GLenum texTarget = GL_TEXTURE_2D);
	Texture(GLenum format, GLenum pixelType, unsigned int width, unsigned int height, GLenum texTarget = GL_TEXTURE_2D);	// Used for framebuffer Color attachment
	Texture(std::array<std::string, 6> paths, const char* texType, GLenum texTarget, GLenum slot, GLenum format, GLenum pixelType); // Used for Cube Map Environment texture
	Texture(unsigned char* color_data, const char* texType, GLuint slot); // Used for simple 1x1 color textures
	~Texture();

	// Sends texture unit to shader
	void sendUniformToShader(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	GLuint getID();

private:

	// Number refering to texture
	GLuint texID;
	// Texture type;
	const char* type;
	// Texture Target
	GLenum texTarget = GL_TEXTURE_2D;
	// Texture unit
	GLuint unit = 0;

	// Texture unit Mapping
	unordered_map<GLuint, GLenum> texUnitMap = {
		{0, GL_TEXTURE0},
		{1, GL_TEXTURE1},
		{2, GL_TEXTURE2},
		{3, GL_TEXTURE3},
		{4, GL_TEXTURE4},
		{5, GL_TEXTURE5},
		{6, GL_TEXTURE6},
		{7, GL_TEXTURE7},
		{8, GL_TEXTURE8},
		{9, GL_TEXTURE9},
		{10, GL_TEXTURE10},
	};
};
