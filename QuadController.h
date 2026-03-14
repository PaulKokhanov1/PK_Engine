#pragma once

#include<glm/glm.hpp>

#include "MeshComponent.h"
#include "InputManager.h"
#include "MathCommon.h"
#include "Application.h"


class QuadController
{
public:
	QuadController(std::unique_ptr<MeshComponent> q);
	~QuadController();

	void update(float deltaTime);

	std::unique_ptr<MeshComponent>& getMesh();

private:

	std::unique_ptr<MeshComponent> quad;
	static constexpr float QUAD_ROTATION_SENSITIVITY = 0.1f;
	static constexpr float SCALE_SENSITIVITY = 0.001f;
	static constexpr float SCALE_MINIMUM = 0.001f;
};
