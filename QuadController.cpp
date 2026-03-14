#include "QuadController.h"

QuadController::QuadController(std::unique_ptr<MeshComponent> q) : quad(move(q)) {}

QuadController::~QuadController()
{
}

void QuadController::update(float deltaTime)
{
	InputManager* input = Application::Get().getInputManager();

	// Right mouse button and drag -> rotate quad 
	if (input->isMouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT)) {

		// Hide cursor
		input->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Trying directly manipulating quaternions
		float yaw = QUAD_ROTATION_SENSITIVITY * input->getDeltaMouseX();
		float pitch = QUAD_ROTATION_SENSITIVITY * input->getDeltaMouseY();

		glm::quat qPitch = glm::angleAxis(pitch, constants::yAxis);
		glm::quat qYaw = glm::angleAxis(yaw, constants::xAxis);

		quad->rotation = qYaw * qPitch * quad->rotation;

		quad->rotation = glm::normalize(quad->rotation);	// Prevent numerical drift
	}
	else if (input->isMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT)) {

		// Unhides cursor since camera is not looking around anymore
		input->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// Left mouse button and drag -> adjust quad scale
	if (input->isMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {

		// Hide cursor
		input->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		quad->scale += (float)input->getDeltaMouseY() * SCALE_SENSITIVITY;
		quad->scale = max(quad->scale, SCALE_MINIMUM);	// Avoid zero or negatives
	}
	else if (input->isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {

		// Unhides cursor since camera is not looking around anymore
		input->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

}

std::unique_ptr<MeshComponent>& QuadController::getMesh()
{
	return quad;
}
