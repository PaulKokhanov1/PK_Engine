#include "LightController.h"


LightController::LightController()
{
}

LightController::~LightController()
{
}

void LightController::update(float deltaTime, std::vector<std::unique_ptr<Light>>& lights)
{
	// CURRENTLY ONLY ACTIVE IF HOLDING DOWN LEFT CTRL

	InputManager* input = Application::Get().getInputManager();

	// Currently we'll adjust each light in this way
	for (auto& light : lights) {

		// Maybe implement more intrakit movement later, maybe move position depending on Camera's orientation?
		if (input->isKeyHeld(GLFW_KEY_W)) light->position.x += LIGHT_MOVEMENT_SPEED * deltaTime;
		if (input->isKeyHeld(GLFW_KEY_S)) light->position.x -= LIGHT_MOVEMENT_SPEED * deltaTime;
		if (input->isKeyHeld(GLFW_KEY_C)) light->position.y += LIGHT_MOVEMENT_SPEED * deltaTime;
		if (input->isKeyHeld(GLFW_KEY_Z)) light->position.y -= LIGHT_MOVEMENT_SPEED * deltaTime;
		if (input->isKeyHeld(GLFW_KEY_A)) light->position.z += LIGHT_MOVEMENT_SPEED * deltaTime;
		if (input->isKeyHeld(GLFW_KEY_D)) light->position.z -= LIGHT_MOVEMENT_SPEED * deltaTime;

		// Set lightMesh position
		if (light->lightMesh && light->shouldShowMesh) {
			light->lightMesh->getTransform().translation = light->position;
		}

		// Have to later change this switch statement to a better design, currently don't love the last min desicion making for sake of cache friendliness

		// Adjust position of light around origin
		if (input->isMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {

			light->yaw += LIGHT_MOVEMENT_SENSITIVITY * input->getDeltaMouseX();
			light->pitch += LIGHT_MOVEMENT_SENSITIVITY * input->getDeltaMouseY();

			float dirX = cos(glm::radians(light->yaw)) * cos(glm::radians(light->pitch));
			float dirY = -sin(glm::radians(light->pitch));
			float dirZ = sin(glm::radians(light->yaw)) * cos(glm::radians(light->pitch));

			switch (light->lightType) {
				case(LightType::POINT):
					break;

				case(LightType::DIRECTIONAL):

					light->direction.x = dirX;
					light->direction.y = dirY;
					light->direction.z = dirZ;

					break;

				case(LightType::SPOT):

					light->direction.x = dirX;
					light->direction.y = dirY;
					light->direction.z = dirZ;

					break;
			}
			light->direction = glm::normalize(light->direction);
			glm::vec3 F = -light->direction;

			// Build orthonormal basis
			if (abs(glm::dot(worldUp, F) > 0.99f)) worldUp = glm::vec3(1.0f, 0.0f, 0.0f);	// So that R vector is not undefined
			glm::vec3 R = glm::normalize(glm::cross(worldUp, F));
			glm::vec3 U = glm::normalize(glm::cross(F, R));	// True up vector wrt light direction

			glm::mat3 rotMatrix(R, U, F);
			glm::quat rot = glm::quat_cast(rotMatrix);

			// Set lightMesh direction
			if (light->lightMesh && light->shouldShowMesh) {
				light->lightMesh->getTransform().rotation = light->lightType == LightType::POINT ? glm::quat(1.0f, 0.0f, 0.0f, 0.0f) : rot;
			}

		}

	}
}

