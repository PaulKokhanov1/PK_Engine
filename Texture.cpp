#include "Texture.h"

Texture::Texture(const char* filename, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = texType;
	unit = slot;

	// Take width, height of incoming img
	unsigned int width, height;

	// setup img data
	std::vector<unsigned char> img_data;
	unsigned err = lodepng::decode(img_data, width, height, filename);	// each row of img_data is 4 bytes, so 1 bytes per channel

	// Handle error
	if (err) {
		LogTextureError( "Decoder Error: " + lodepng_error_text(err));
	}

	// Generate Texture
	glGenTextures(1, &texID);

	glActiveTexture(texUnitMap[unit]); // texture unit
	glBindTexture(GL_TEXTURE_2D, texID);	// binds texture to texture unit: slot

	// Set texture parameters so we use bi-linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// To avoid "dancing pixels"
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// How texture coordinates are handled outside of region 0,0 to 1,1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Assign img data to binded texture
	glTexImage2D(
		GL_TEXTURE_2D,			// typically will be GL_TEXTURE_2D, texture, but there are multiple 2D texture format's
		0,					// Mipmap level, meaning highest resolution img
		GL_RGBA,			// Internal Format
		width,
		height,
		0,				// Border, just keep at ZERO
		format,			// Format, i.e what color's img will support
		pixelType,		// Data type of format, i.e "each color is of what type?"
		img_data.data()
	);

	// Generate Mipmap levels
	glGenerateMipmap(GL_TEXTURE_2D);


	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[OpenGL] Error creating Texture (code " << error << ")\n";
	}

}

Texture::Texture(GLenum format, GLenum pixelType, unsigned int width, unsigned int height)
{
	unit = 0;
	// Generate Texture
	glGenTextures(1, &texID);
	glActiveTexture(texUnitMap[unit]); // texture unit
	glBindTexture(GL_TEXTURE_2D, texID);

	// Set texture parameters so we use bi-linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// To avoid "dancing pixels"
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// How texture coordinates are handled outside of region 0,0 to 1,1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Assign img data to binded texture
	glTexImage2D(
		GL_TEXTURE_2D,			// typically will be GL_TEXTURE_2D, texture, but there are multiple 2D texture format's
		0,					// Mipmap level, meaning highest resolution img
		GL_RGBA8,			// Internal Format
		width,
		height,
		0,				// Border, just keep at ZERO
		GL_RGBA,			// Format, i.e what color's img will support
		pixelType,		// Data type of format, i.e "each color is of what type?"
		0
	);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[OpenGL] Error creating Texture (code " << error << ")\n";
	}
}

Texture::~Texture()
{
	Delete();
}

void Texture::sendUniformToShader(Shader& shader, const char* uniform, GLuint unit)
{
	// Location of uniform
	GLuint sampler = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	// Set value of uniform
	glUniform1i(sampler, unit);
}

void Texture::Bind()
{
	glActiveTexture(texUnitMap[unit]); // texture unit
	glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &texID);
}

GLuint Texture::getID()
{
	return texID;
}
