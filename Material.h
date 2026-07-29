#pragma once

#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<string>
#include<set>

#include"EngineConfig.h"
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
	~Material() = default;

	void loadTextures();
	void uploadData(Shader& shader);

	void setAttributes(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess);
	void setTexturePaths(std::string filepath, const char* map_ka, const char* map_kd, const char* map_ks, const char* map_norm, const char* map_disp);
	void setShaderName(const std::string& shaderName);
	void setNormalMapPath(const char* normalMapPath);
	void setDisplacementMapPath(const char* displacementMapPath);

	// Get Shader, preventing change of Shader Name
	std::string getShaderName() const;

private:
	void loadTexture(GLenum format, GLint internalFormat, GLenum pixelType, GLenum target, const std::string& map, Texture*& loadedTexture, const std::string& fallbackTexture);

	// Common material attributes, vector's hold color value
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	std::string map_ka;
	std::string map_kd;
	std::string map_ks;
	std::string map_norm;
	std::string map_disp;
	Texture* loadedDiffuseTexture = nullptr;
	Texture* loadedAmbientTexture = nullptr;
	Texture* loadedSpecularTexture = nullptr;
	Texture* loadedNormalMapTexture = nullptr;
	Texture* loadedDisplacementMapTexture = nullptr;

	// Reference to used shader for material
	std::string shaderName;

};
