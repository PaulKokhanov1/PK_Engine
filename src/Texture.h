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
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// Different Texture loading options
	void Load2D(const char* filename, GLenum format, GLint internalFormat, GLenum pixelType, GLenum texTarget = GL_TEXTURE_2D);
	void CreateRenderTarget(unsigned int width, unsigned int height, GLenum format, GLint internalFormat, GLenum pixelType, const std::vector<std::pair<GLenum, GLint>>& texParameters, GLenum texTarget = GL_TEXTURE_2D);
	void LoadCubeMap(const std::array<std::string, 6>& paths, GLenum texTarget, GLenum format, GLenum pixelType);
	void LoadDepthCubeMap(unsigned int width, unsigned int height,  GLenum format, GLint internalFormat, GLenum pixelType, const std::vector<std::pair<GLenum, GLint>>& texParameters, GLenum texTarget = GL_TEXTURE_CUBE_MAP);
	void CreateFallback(unsigned char* color_data);

	// Binds a texture
	void Bind(uint32_t slot);
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	GLuint getID();

private:

	// Number refering to texture
	GLuint texID = 0;
	// Texture Target
	GLenum texTarget = GL_TEXTURE_2D;
};
