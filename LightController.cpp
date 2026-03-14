#include "LightController.h"


LightController::LightController()
{
}

LightController::~LightController()
{
}

void LightController::update(float deltaTime, std::vector<std::unique_ptr<Light>>& lights)
{
	InputManager* input = Application::Get().getInputManager();

	// Adjust position of light around origin
	if (input->isMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {

		// Currently we'll adjust each light in this way
		for (auto& light : lights) {

			light->yaw += LIGHT_MOVEMENT_SENSITIVITY * input->getDeltaMouseX();
			light->pitch += LIGHT_MOVEMENT_SENSITIVITY * input->getDeltaMouseY();

			float dirX = cos(glm::radians(light->yaw)) * cos(glm::radians(light->pitch));
			float dirY = -sin(glm::radians(light->pitch));
			float dirZ = sin(glm::radians(light->yaw)) * cos(glm::radians(light->pitch));


			light->position.x = light->radius * dirX;
			//light->position.y = light->radius * dirY;
			light->position.z = light->radius * dirZ;


		}

	}
}

