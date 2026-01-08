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

	vector<Texture*> outputDif;
	for (auto t : loadedDiffuseTextures) {
		outputDif.push_back(t.second);
	}	

	vector<Texture*> outputAmb;
	for (auto t : loadedAmbientTextures) {
		outputAmb.push_back(t.second);
	}	

	vector<Texture*> outputSpec;
	for (auto t : loadedSpecularTextures) {
		outputSpec.push_back(t.second);
	}

	return TextureData{
		outputDif,
		outputAmb,
		outputSpec
	};
}

void Material::loadTextures()
{
	// Once using multiple textures per material, will need to hold a vector of textures are parse and load each

	// Load Diffuse texture
	if (loadedDiffuseTextures.find(map_kd) == loadedDiffuseTextures.end()) {
		Texture* difTex = new Texture(map_kd.c_str(), GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
		loadedDiffuseTextures[map_kd] = difTex;
	}

	// Load Ambient texture
	if (loadedAmbientTextures.find(map_ka) == loadedAmbientTextures.end()) {
		Texture* ambTex = new Texture(map_ka.c_str(), GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE);
		loadedAmbientTextures[map_ka] = ambTex;
	}

	// Load Specular texture
	if (loadedSpecularTextures.find(map_ks) == loadedSpecularTextures.end()) {
		Texture* specTex = new Texture(map_ks.c_str(), GL_TEXTURE_2D, 2, GL_RED, GL_UNSIGNED_BYTE);
		loadedSpecularTextures[map_ks] = specTex;
	}
}

void Material::setTextures(const char* map_ka, const char* map_kd, const char* map_ks)
{
	this->map_ka = map_ka;
	this->map_kd = map_kd;
	this->map_ks = map_ks;
}
