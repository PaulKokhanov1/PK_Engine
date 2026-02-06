#pragma once

#include<glm/glm.hpp>

#include "MeshComponent.h"
#include "InputManager.h"
#include "MathCommon.h"


class QuadController
{
public:
	QuadController(MeshComponent* q);
	~QuadController();

	void update(InputManager& input, float deltaTime);

	MeshComponent* getMesh();

private:

	MeshComponent* quad;
	static constexpr float QUAD_ROTATION_SENSITIVITY = 0.1f;
	static constexpr float SCALE_SENSITIVITY = 0.001f;
	static constexpr float SCALE_MINIMUM = 0.001f;
};
