#pragma once

#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<string>
#include<set>

#include"TextureManager.h"
#include"Application.h"

struct MaterialData
{
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	float shininess;
};

class Material
{
public:
	Material(std::string shaderName = "default");
	~Material();

	void loadTextures();
	void uploadData(Shader& shader);

	void setAttributes(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess);
	void setTexturePaths(std::string filepath, const char* map_ka, const char* map_kd, const char* map_ks);
	void setShaderName(std::string shaderName);

	// Get Shader, preventing change of Shader Name
	std::string getShaderName() const;
	MaterialData getAttributes();

private:

	// Common material attributes, vector's hold color value
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	std::string map_ka;
	std::string map_kd;
	std::string map_ks;
	Texture* loadedDiffuseTexture = nullptr;
	Texture* loadedAmbientTexture = nullptr;
	Texture* loadedSpecularTexture = nullptr;

	// Reference to used shader for material
	std::string shaderName;

};
