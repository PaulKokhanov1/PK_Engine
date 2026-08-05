#include "Material.h"

Material::Material(std::string shaderName) : 
	shaderName(shaderName),
	diffuse(glm::vec3(1.0f, 1.0f, 1.0f)), 
	ambient(glm::vec3(1.0f, 1.0f, 1.0f)), 
	specular(glm::vec3(1.0f, 1.0f, 1.0f)), 
	shininess(100.f),
	map_ka(""),
	map_kd(""),
	map_ks(""),
	map_norm(""),
	map_disp("")

{}

void Material::setAttributes(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess)
{
	ambient = Ka;
	diffuse = Kd;
	specular = Ks;
	this->shininess = shininess;
}

std::string Material::getShaderName() const
{
	return shaderName;
}

void Material::loadTexture(GLenum format, GLint internalFormat, GLenum pixelType, GLenum target, const std::string& map, Texture*& loadedTexture, const std::string& fallbackTexture)
{
	TextureManager* texManager = Application::Get().getTextureManager();

	TextureDescriptor texDesc;
	texDesc.path = map.c_str();
	texDesc.format = format;
	texDesc.target = target;
	texDesc.pixelType = pixelType;
	texDesc.internalFormat = internalFormat;
	if (!map.empty()) loadedTexture = texManager->load(texDesc);
	else if (!loadedTexture) loadedTexture = texManager->getFallback(fallbackTexture);
}

void Material::loadTextures()
{
	// Each material holds 1 type of texture

	// Loading textures, assuming one sampler type per shader, fallback to default texture, if no texture is specified and texture for that type is not already loaded
	// Prior to load regular texture, it does not know width and height, so store as 0 (Refering to TextureDescriptor Parameters)
	
	// Load Diffuse texture
	loadTexture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, map_kd, loadedDiffuseTexture, "gray");

	// Load Ambient texture
	loadTexture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, map_ka, loadedAmbientTexture, "gray");

	// Load Specular texture
	loadTexture(GL_RED, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, map_ks, loadedSpecularTexture, "black");

	// Load Normal Maps
	loadTexture(GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, map_norm, loadedNormalMapTexture, "black");

	// Load Displacement Maps
	loadTexture(GL_RGBA, GL_RED, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, map_disp, loadedDisplacementMapTexture, "black");
}

void Material::uploadData(Shader& shader)
{
	shader.setUniform3fv("Ka", ambient);
	shader.setUniform3fv("Kd", diffuse);
	shader.setUniform3fv("Ks", specular);
	shader.setUniform1f("shininess", shininess);

	// Upload textures to Vertex Shader

	if (loadedDiffuseTexture) {
		shader.setSampler("texDiffuse", textureSlots::DIFFUSE);
		loadedDiffuseTexture->Bind(textureSlots::DIFFUSE);
	}

	if (loadedAmbientTexture) {
		shader.setSampler("texAmbient", textureSlots::AMBIENT);
		loadedAmbientTexture->Bind(textureSlots::AMBIENT);
	}

	if (loadedSpecularTexture) {
		shader.setSampler("texSpecular", textureSlots::SPECULAR);
		loadedSpecularTexture->Bind(textureSlots::SPECULAR);
	}

	if (loadedNormalMapTexture) {
		shader.setSampler("texNormal", textureSlots::NORMAL);
		loadedNormalMapTexture->Bind(textureSlots::NORMAL);
	}
		
	if (loadedDisplacementMapTexture) {
		shader.setSampler("texDisplacement", textureSlots::DISPLACEMENT);
		loadedDisplacementMapTexture->Bind(textureSlots::DISPLACEMENT);
	}

}

void Material::setTexturePaths(std::string filepath, const char* map_ka, const char* map_kd, const char* map_ks, const char* map_norm, const char* map_disp)
{
	if (map_ka) this->map_ka = filepath + map_ka;
	if (map_kd) this->map_kd = filepath + map_kd;
	if (map_ks) this->map_ks = filepath + map_ks;
	if (map_norm) this->map_norm = filepath + map_norm;
	if (map_disp) this->map_disp = filepath + map_disp;
}

void Material::setShaderName(const std::string& shaderName)
{
	this->shaderName = shaderName;
}

void Material::setNormalMapPath(const char* normalMapPath)
{
	if (!normalMapPath) return;
	map_norm = normalMapPath;
	loadTextures();
}

void Material::setDisplacementMapPath(const char* displacementMapPath)
{
	if (!displacementMapPath) return;
	map_disp = displacementMapPath;
	loadTextures();
}
