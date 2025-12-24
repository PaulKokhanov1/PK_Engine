#pragma once

#include<glm/glm.hpp>

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
	Material();
	~Material();

	void setAttributes(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess);
	MaterialData getAttributes();

private:

	// Common material attributes, vector's hold color value
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float shininess;

};
