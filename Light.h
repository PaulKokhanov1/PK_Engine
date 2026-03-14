#pragma once

#include<glm/glm.hpp>
#include<limits>

#include"LogLight.h"
#include"MathCommon.h"

enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOT
};

// Point Light for Now
struct Light
{
	LightType lightType;

	glm::vec3 position;
	glm::vec3 color;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float envLightIntensity;	// This should probably not be in the Light class

	float yaw = -90.0f;
	float pitch = 0.0f;

	// Point Light Specific
	float radius = 6.0f; // TEMPORARY FOR TESTING

	// SpotLight Specific
	float innerCone;
	float outerCone;

	Light() {}

	Light(
		LightType type, glm::vec3 col, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, 
		glm::vec3 pos, float envIntensity, 
		float range = 0.0f, float inCone = 0.0f, float outCone = 0.0f) : lightType(type), color(col), ambient(amb), diffuse(diff), specular(spec), position(pos), envLightIntensity(envIntensity), 
		radius(range), innerCone(inCone), outerCone(outCone)
	{}

	bool dirty = true;

	bool validate();
};