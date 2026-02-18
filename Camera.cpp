#include "Camera.h"

Camera::Camera(int w, int h, glm::vec3 pos, float fieldOfView, float nearP, float farP) : width(w), height(h), Position(pos), FOV(fieldOfView), nearPlane(nearP), farPlane(farP) {}

void Camera::updateViewProjection()
{
	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Set projection
	projection = getProjectionMatrix();
}

void Camera::sendViewAndProjToShader(Shader& shader)
{
	// Ensure uniform location exists for View and Projection
	GLint viewMatrixLocation = glGetUniformLocation(shader.ID, "viewMatrix");
	if (viewMatrixLocation == -1) {
		LogCameraWarn("Uniform viewMatrix not found in shader.");
		return;
	}	
	
	GLint projectionMatrixLocation = glGetUniformLocation(shader.ID, "projectionMatrix");
	if (projectionMatrixLocation == -1) {
		LogCameraWarn("Uniform projectionMatrix not found in shader.");
		return;
	}


	// Exports View and Projection matrix
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::sendCamDistanceScaleToShader(Shader& shader)
{
	GLint camDistanceScaleLocation = glGetUniformLocation(shader.ID, "camDistanceScale");
	if (camDistanceScaleLocation == -1) {
		LogCameraWarn("Uniform camDistanceScale not found in shader.");
		return;
	}

	glUniform1f(camDistanceScaleLocation, getDistanceScale());
}

void Camera::sendCamPositionWorldSpaceToShader(Shader& shader)
{
	GLint camPosWorldLocation = glGetUniformLocation(shader.ID, "camPosWorld");
	if (camPosWorldLocation == -1) {
		LogCameraWarn("Uniform camPosWorld not found in shader.");
		return;
	}

	// Exports View and Projection matrix
	glUniform3fv(camPosWorldLocation, 1, glm::value_ptr(Position));
}

void Camera::updateInputs(InputManager& input, float dt)
{
	// Handle keyboard inputs
	if (input.isKeyHeld(GLFW_KEY_W)) Position += speed * Orientation * dt;
	if (input.isKeyHeld(GLFW_KEY_S)) Position -= speed * Orientation * dt;
	if (input.isKeyHeld(GLFW_KEY_A)) Position -= glm::normalize(glm::cross(Orientation, Up)) * speed * dt;
	if (input.isKeyHeld(GLFW_KEY_D)) Position += glm::normalize(glm::cross(Orientation, Up)) * speed * dt;

	// Switch between perspective and orthogonal transformation
	if (input.isKeyPressed(GLFW_KEY_P)) {
		isPerspective = !isPerspective;
		if (!isPerspective) {
			referenceDistance = glm::length(Position);
		}
		projectionDirty = true;
	}


	// Right mouse button and drag -> adjust camera angles
	if (input.isMouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT)) {

		// Hide cursor
		input.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		yaw += sensitivity * input.getDeltaMouseX();
		pitch += sensitivity * input.getDeltaMouseY();

		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		// Calculate orientation based on trigonometric relations using angle moved for respective axis
		Orientation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		Orientation.y = -sin(glm::radians(pitch));
		Orientation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		// Ensure Orientation is always a unit vector
		Orientation = glm::normalize(Orientation);

	}
	else if (input.isMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT)) {

		// Unhides cursor since camera is not looking around anymore
		input.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// Left mouse button and drag -> adjust camera distance
	if (input.isMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {

		Position += (speed  / 10.f) * (float)input.getDeltaMouseY() * Orientation;
	}
	else if (input.isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {

		// Unhides cursor since camera is not looking around anymore
		input.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

const glm::vec3& Camera::getPosition() const
{
	return Position;
}

const glm::vec3& Camera::getOrientation() const
{
	return Orientation;
}

const glm::mat4 Camera::getProjectionMatrix() const
{
	if (projectionDirty) {
		recomputeProjection();
		projectionDirty = false;
	}
	return projection;

}

const glm::mat4 Camera::getViewMatrix() const
{
	return view;
}

float Camera::getDistanceScale() const
{
	float distance = glm::length(Position);
	if (distance <= 1e-6f) {
		return 1.0f;
	}
	return isPerspective ? 1 : (referenceDistance / distance);
}

inline float Camera::getAspectRatio() const
{
	return static_cast<float>(width) / std::max(1, height);
}

void Camera::setFOV(float newFOV)
{
	FOV = newFOV; 
	projectionDirty = true;
}

void Camera::setClipPlanes(float nearP, float farP)
{
	nearPlane = nearP;
	farPlane = farP;
	projectionDirty = true;
}

void Camera::setScreenDimensions(int w, int h)
{
	width = w;
	height = h;
	projectionDirty = true;
}

void Camera::recomputeProjection() const
{
	// create Projection matrix for perspective projection
	if (isPerspective) {
		projection = glm::perspective(glm::radians(FOV), getAspectRatio(), nearPlane, farPlane);
		return;
	}

	// Multiply by aspectRatio to have equal proportions
	float halfHeight = referenceDistance * tanf(glm::radians(FOV) / 2);
	float halfWidth = halfHeight * getAspectRatio(); 

	projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
	return;
}
