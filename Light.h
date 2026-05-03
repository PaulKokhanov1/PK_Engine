#pragma once

#include<glm/glm.hpp>
#include<limits>

#include"LogLight.h"
#include"MathCommon.h"
#include"Shader.h"
#include"MeshComponent.h"

enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOT
};

// Point Light for Now
struct Light
{
	LightType lightType;

	glm::vec3 color = glm::vec3(0.0f);
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f);

	float envLightIntensity;	// This should probably not be in the Light class

	float yaw = -90.0f;			// Move this elsewhere, ex LightController
	float pitch = 0.0f;

	// SpotLight Specific
	float innerCone = 0.f;
	float outerCone = 0.f;

	bool shouldShowMesh = false;

	Light() {}

	Light(
		LightType type, glm::vec3 col, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, 
		glm::vec3 pos, glm::vec3 dir, float envIntensity, 
		float inCone = 0.0f, float outCone = 0.0f, bool showMesh = true) : lightType(type), color(col), ambient(amb), diffuse(diff), specular(spec), position(pos), direction(dir), envLightIntensity(envIntensity),
		innerCone(inCone), outerCone(outCone),
		shouldShowMesh(showMesh)
	{}

	bool dirty = true;

	bool validate();

	void sendLightDataToShader(Shader& shader);
	void setlightMesh(std::unique_ptr<MeshComponent> lM);
	MeshComponent* getMesh();

	std::unique_ptr<MeshComponent> lightMesh;
};