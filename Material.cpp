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
{
}

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

TextureData Material::getTextures()
{
	return TextureData{
		loadedDiffuseTexture,
		loadedAmbientTexture,
		loadedSpecularTexture
	};
}

void Material::loadTextures(const char* filepath)
{
	// Each material holds 1 type of texture

	// Load Diffuse texture
	
	if (map_ka != "") loadedDiffuseTexture = new Texture((filepath + map_kd).c_str(), "Diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);

	// Load Ambient texture
	if (map_kd != "") loadedAmbientTexture = new Texture((filepath + map_ka).c_str(), "Ambient", 1, GL_RGBA, GL_UNSIGNED_BYTE);

	// Load Specular texture
	if (map_ks != "") loadedSpecularTexture = new Texture((filepath + map_ks).c_str(), "Specular", 2, GL_RED, GL_UNSIGNED_BYTE);
}

void Material::setTextures(const char* map_ka, const char* map_kd, const char* map_ks)
{
	if (map_ka) this->map_ka = map_ka;
	if (map_kd) this->map_kd = map_kd;
	if (map_ks) this->map_ks = map_ks;
}
