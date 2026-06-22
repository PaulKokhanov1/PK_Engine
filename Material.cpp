#include "Material.h"

Material::Material(std::string shaderName) : 
	shaderName(shaderName),
	diffuse(glm::vec3(1.0f, 1.0f, 1.0f)), 
	ambient(glm::vec3(1.0f, 1.0f, 1.0f)), 
	specular(glm::vec3(1.0f, 1.0f, 1.0f)), 
	shininess(100.f),
	map_ka(""),
	map_kd(""),
	map_ks("")

{}

Material::~Material()
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

MaterialData Material::getAttributes()
{
	return MaterialData{
		ambient,
		diffuse,
		specular,
		shininess,
	};
}

void Material::loadTextures()
{
	// Each material holds 1 type of texture
	TextureManager* texManager = Application::Get().getTextureManager();

	// Loading textures, assuming one sampler type per shader, fallback to default texture, if no texture is specified and texture for that type is not already loaded
	// Prior to load regular texture, it does not know with and height, so store as 0 (Refering to TextureDescriptor Parameters)
	
	// Load Diffuse texture
	TextureDescriptor diffTexDesc;
	diffTexDesc.path = map_kd.c_str();
	diffTexDesc.format = GL_RGBA;
	diffTexDesc.target = GL_TEXTURE_2D;
	diffTexDesc.pixelType = GL_UNSIGNED_BYTE;
	if (!map_kd.empty()) loadedDiffuseTexture = texManager->load(diffTexDesc);
	else if (!loadedDiffuseTexture) loadedDiffuseTexture = texManager->getFallback("gray");

	// Load Ambient texture
	TextureDescriptor ambTexDesc;
	ambTexDesc.path = map_ka.c_str();
	ambTexDesc.format = GL_RGBA;
	ambTexDesc.target = GL_TEXTURE_2D;
	ambTexDesc.pixelType = GL_UNSIGNED_BYTE;
	if (!map_ka.empty())  loadedAmbientTexture = texManager->load(ambTexDesc);
	else if (!loadedAmbientTexture) loadedAmbientTexture = texManager->getFallback("gray");

	// Load Specular texture
	TextureDescriptor specTexDesc;
	specTexDesc.path = map_ks.c_str();
	specTexDesc.format = GL_RED;
	specTexDesc.target = GL_TEXTURE_2D;
	specTexDesc.pixelType = GL_UNSIGNED_BYTE;
	if (!map_ks.empty())  loadedSpecularTexture = texManager->load(specTexDesc);
	else if (!loadedSpecularTexture) loadedSpecularTexture = texManager->getFallback("gray");

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

}

void Material::setTexturePaths(std::string filepath, const char* map_ka, const char* map_kd, const char* map_ks)
{
	if (map_ka) this->map_ka = filepath + map_ka;
	if (map_kd) this->map_kd = filepath + map_kd;
	if (map_ks) this->map_ks = filepath + map_ks;
}

void Material::setShaderName(std::string shaderName)
{
	this->shaderName = shaderName;
}
