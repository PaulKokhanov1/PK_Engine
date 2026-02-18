#include "Texture.h"

Texture::Texture(const char* filename, const char* texType, GLuint slot, GLenum format, GLenum pixelType, GLenum texTarget) : texTarget(texTarget), type(texType), unit(slot)
{
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
	glBindTexture(texTarget, texID);	// binds texture to texture unit: slot

	// Set texture parameters so we use bi-linear interpolation
	glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// To avoid "dancing pixels"
	glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// How texture coordinates are handled outside of region 0,0 to 1,1
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Assign img data to binded texture
	glTexImage2D(
		texTarget,			// typically will be GL_TEXTURE_2D, texture, but there are multiple 2D texture format's
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
	glGenerateMipmap(texTarget);


	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[OpenGL] Error creating Texture (code " << error << ")\n";
	}

}

Texture::Texture(GLenum format, GLenum pixelType, unsigned int width, unsigned int height, GLenum texTarget) : texTarget(texTarget), unit(0)
{
	// Generate Texture
	glGenTextures(1, &texID);
	glActiveTexture(texUnitMap[unit]); // texture unit
	glBindTexture(texTarget, texID);

	// Set texture parameters so we use bi-linear interpolation
	glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// To avoid "dancing pixels"
	glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// How texture coordinates are handled outside of region 0,0 to 1,1
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Assign img data to binded texture
	glTexImage2D(
		texTarget,			// typically will be GL_TEXTURE_2D, texture, but there are multiple 2D texture format's
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

Texture::Texture(std::array<std::string, 6> paths, const char* texType, GLenum texTarget, GLenum slot, GLenum format, GLenum pixelType) : texTarget(texTarget), type(texType), unit(slot)
{

	// Generate Texture
	glGenTextures(1, &texID);

	glActiveTexture(texUnitMap[unit]); // texture unit
	glBindTexture(texTarget, texID);	// binds texture to texture unit: slot

	// Set texture parameters so we use bi-linear interpolation
	glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// To avoid "dancing pixels"
	glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// How texture coordinates are handled outside of region 0,0 to 1,1
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
	// Setup Img data for each side of cubemap

	for (unsigned int i = 0; i < 6; ++i) {
		// Take width, height of incoming img
		unsigned int width, height;
		std::vector<unsigned char> img_data;
		unsigned err = lodepng::decode(img_data, width, height, paths[i]);	// each row of img_data is 4 bytes, so 1 bytes per channel

		// Handle error
		if (err) {
			LogTextureError("Decoder Error: " + lodepng_error_text(err));
		}

		// Only proceed if there is data
		if (img_data.data()) {

			// Assign img data to binded texture
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,// MUST pass in cube map images appropriately, +X ->right, -X ->left, +Y -> top, -Y -> bot, +Z -> front, -Z -> back
				0,					// Mipmap level, meaning highest resolution img
				GL_RGBA,			// Internal Format
				width,
				height,
				0,				// Border, just keep at ZERO
				format,			// Format, Cube Map SHOULD be GL_RGBA in general
				pixelType,		// Data type of format, Cube Map should be GL_UNSIGNED_BYTE in general
				img_data.data()
			);

		}

	}

	// Generate Mipmap levels
	glGenerateMipmap(texTarget);

	// Enable Seamless global flag to avoid seams due to bi-linear filtering near corners or edges
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[OpenGL] Error creating Cube Map Texture (code " << error << ")\n";
	}
}

Texture::Texture(unsigned char* color_data, const char* texType, GLuint slot) : type(texType), unit(slot)
{
	// Generate Texture
	glGenTextures(1, &texID);

	glActiveTexture(texUnitMap[unit]); // texture unit
	glBindTexture(GL_TEXTURE_2D, texID);	// binds texture to texture unit: slot

	// Set texture parameters so we use bi-linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// How texture coordinates are handled outside of region 0,0 to 1,1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Assign img data to binded texture
	glTexImage2D(
		GL_TEXTURE_2D,			// typically will be GL_TEXTURE_2D, texture, but there are multiple 2D texture format's
		0,					// Mipmap level, meaning highest resolution img
		GL_RGBA,			// Internal Format
		1,
		1,
		0,				// Border, just keep at ZERO
		GL_UNSIGNED_BYTE,			// Format, i.e what color's img will support
		GL_RGBA,		// Data type of format, i.e "each color is of what type?"
		color_data
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
	glBindTexture(texTarget, texID);
}

void Texture::Unbind()
{
	glBindTexture(texTarget, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &texID);
}

GLuint Texture::getID()
{
	return texID;
}
