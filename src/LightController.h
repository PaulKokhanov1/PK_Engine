#pragma once

#include<vector>
#include<glm/glm.hpp>

#include "Light.h"
#include "InputManager.h"
#include "Application.h"

class LightController
{
public:
	LightController();
	~LightController();

	// LightController should only reference the lights (though not actually sure, I guess it makes sense since we can dnamically add light in our scene)
	void update(float deltaTime, std::vector<std::unique_ptr<Light>>& lights);

private:

	// Light movement sensitivity
	float LIGHT_MOVEMENT_SENSITIVITY = 1.0f;
	float LIGHT_MOVEMENT_SPEED = 1.0f;

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);


};
