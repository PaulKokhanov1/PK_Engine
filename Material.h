#pragma once

#include<glm/glm.hpp>
#include<string>
#include<set>

#include"Texture.h"


struct MaterialData
{
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	float shininess;
};

struct TextureData
{
	vector<Texture*> diffuseTextures;
	vector<Texture*> ambientTextures;
	vector<Texture*> specularTextures;
};

class Material
{
public:
	Material(std::string shaderName = "default");
	~Material();

	void loadTextures();

	void setAttributes(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess);
	void setTextures(const char* map_ka, const char* map_kd, const char* map_ks);

	// Get Shader, preventing change of Shader Name
	std::string getShaderName() const;
	MaterialData getAttributes();
	TextureData getTextures();

private:

	// Common material attributes, vector's hold color value
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	std::string map_ka;
	std::string map_kd;
	std::string map_ks;
	unordered_map<string, Texture*> loadedDiffuseTextures;
	unordered_map<string, Texture*> loadedAmbientTextures;
	unordered_map<string, Texture*> loadedSpecularTextures;


	// Reference to used shader for material
	std::string shaderName;

};
