#pragma once

#include<glad/glad.h>
#include <array>

#include"ShaderManager.h"
#include"lodepng.h"
#include"LogTexture.h"

class Texture
{
public:
	Texture();
	~Texture();

	// Different Texture loading options
	void Load2D(const char* filename, GLenum format, GLenum pixelType, GLenum texTarget = GL_TEXTURE_2D);
	void CreateRenderTarget(unsigned int width, unsigned int height, GLenum format, GLint internalFormat, GLenum pixelType, std::vector<std::pair<GLenum, GLint>> texParameters, GLenum texTarget = GL_TEXTURE_2D);
	void LoadCubeMap(std::array<std::string, 6> paths, GLenum texTarget, GLenum format, GLenum pixelType);
	void LoadDepthCubeMap(unsigned int width, unsigned int height,  GLenum format, GLint internalFormat, GLenum pixelType, std::vector<std::pair<GLenum, GLint>> texParameters, GLenum texTarget = GL_TEXTURE_CUBE_MAP);
	void CreateFallback(unsigned char* color_data);

	// Binds a texture
	void Bind(uint32_t slot);
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	GLuint getID();
	const char* getType();

private:

	// Number refering to texture
	GLuint texID;
	// Texture Target
	GLenum texTarget = GL_TEXTURE_2D;

	// Texture unit Mapping
	std::unordered_map<GLuint, GLenum> texUnitMap = {
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
