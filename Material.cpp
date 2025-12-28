#include "Material.h"

Material::Material(std::string shaderName) : 
	shaderName(shaderName),
	diffuse(glm::vec3(1.0f, 1.0f, 1.0f)), 
	ambient(glm::vec3(1.0f, 1.0f, 1.0f)), 
	specular(glm::vec3(1.0f, 1.0f, 1.0f)), 
	shininess(100.f)
{
}

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
		shininess
	};
}
