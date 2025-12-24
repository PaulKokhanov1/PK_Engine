#pragma once

#include<glm/glm.hpp>

// Point Light for Now
struct Light
{
	glm::vec3 position;
	glm::vec3 color;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float yaw = -90.0f;
	float pitch = 0.0f;
	float radius = 6.0f; // TEMPORARY FOR TESTING
};