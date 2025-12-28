#pragma once

#include<glm/glm.hpp>
#include<string>


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

	void setAttributes(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess);

	// Get Shader, preventing change of Shader Name
	std::string getShaderName() const;
	MaterialData getAttributes();

private:

	// Common material attributes, vector's hold color value
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	// Reference to used shader for material
	std::string shaderName;

};
