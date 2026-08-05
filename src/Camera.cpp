#include "Camera.h"

Camera::Camera(int w, int h, glm::vec3 pos, float fieldOfView, float nearP, float farP) : width(w), height(h), Position(pos), FOV(fieldOfView), nearPlane(nearP), farPlane(farP) {}

void Camera::updateViewProjection()
{
	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Set projection
	projection = getProjectionMatrix();
}

void Camera::calcMirroredViewMatrix(glm::vec3 mirrorNormal, glm::vec3 mirrorPos)
{
	// Need "Position", "Orientation", "Up", plane "Normal", and "distance d" of plane
	glm::vec3 N = glm::normalize(mirrorNormal);
	glm::vec3 F = glm::normalize(Orientation);
	glm::vec3 U = glm::normalize(Up);

	// Calculate mirrored Eye Position
	glm::vec3 eyeVector = Position - mirrorPos; // Vector from camera Position to point on plane, arrow should be towards camera pos
	float dist = glm::dot(eyeVector, N); // Get the dist from mirrorPos along its normal, so project vectro onto normal of mirror
	mirroredPosition = Position - 2.0f * dist * N; // Then move eye/camera position 2 * dust in opposite direction of normal (to opposite side of plane)


	// Calculate new oritentation/looking direction
	float distCamDir = glm::dot(F, N); // Create direction of where camera is looking, then find dist along mirror normal
	glm::vec3 mirroredForward = F - 2.0f * (distCamDir)*N; // Calculate resulting vector after moving down along normal of mirror plane, multiply by 2 because vector is MAGNITUDE & Direction, so we move it downwards then we subtract from original direction to get resulting vector

	// Now reflect Up vector, same process as above
	float distUpDir = glm::dot(U, N);
	glm::vec3 mirroredUpVec = U - 2.0f * (distUpDir) * N;

	mirroredView = glm::lookAt(mirroredPosition, mirroredPosition + mirroredForward, mirroredUpVec);
}

void Camera::sendViewAndProjToShader(Shader& shader)
{
	shader.setUniformMat4fv("viewMatrix", view);
	shader.setUniformMat4fv("projectionMatrix", projection);
}

void Camera::sendMirroredViewAndProjToShader(Shader& shader)
{
	shader.setUniformMat4fv("viewMatrix", mirroredView);
	shader.setUniformMat4fv("projectionMatrix", projection);
}

void Camera::sendMirroredViewToShader(Shader& shader)
{
	shader.setUniformMat4fv("reflectionViewMatrix", mirroredView);
}

void Camera::sendInverseProjViewToShader(Shader& shader)
{
	shader.setUniformMat4fv("invProjView", glm::inverse(projection * view));
}

void Camera::sendCamPositionWorldSpaceToShader(Shader& shader)
{
	shader.setUniform3fv("camPosWorld", Position);
}

void Camera::updateInputs(float dt)
{

	InputManager* input = Application::Get().getInputManager();

	// Handle keyboard inputs
	if (input->isKeyHeld(GLFW_KEY_W)) Position += speed * Orientation * dt;
	if (input->isKeyHeld(GLFW_KEY_S)) Position -= speed * Orientation * dt;
	if (input->isKeyHeld(GLFW_KEY_A)) Position -= glm::normalize(glm::cross(Orientation, Up)) * speed * dt;
	if (input->isKeyHeld(GLFW_KEY_D)) Position += glm::normalize(glm::cross(Orientation, Up)) * speed * dt;

	// Switch between perspective and orthogonal transformation
	if (input->isKeyPressed(GLFW_KEY_P)) {
		isPerspective = !isPerspective;
		if (!isPerspective) {
			orthoSize = glm::length(Position) * tanf(glm::radians(FOV) / 2);
		}
		projectionDirty = true;
	}


	// Right mouse button and drag -> adjust camera angles
	if (input->isMouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT)) {

		// Hide cursor
		input->setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		yaw += sensitivity * input->getDeltaMouseX();
		pitch += sensitivity * input->getDeltaMouseY();

		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		// Calculate orientation based on trigonometric relations using angle moved for respective axis
		Orientation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		Orientation.y = -sin(glm::radians(pitch));
		Orientation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		// Ensure Orientation is always a unit vector
		Orientation = glm::normalize(Orientation);

	}
	else if (input->isMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT)) {

		// Unhides cursor since camera is not looking around anymore
		input->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// Left mouse button and drag -> adjust camera distance
	if (input->isMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {

		if (isPerspective) Position += (speed  / 10.f) * (float)input->getDeltaMouseY() * Orientation;
		else {
			orthoSize *= (1 + zoomFactor * (float)input->getDeltaMouseY());
			orthoSize = glm::clamp(orthoSize, 0.01f, 100.0f);
			recomputeProjection();
		}
	}
	else if (input->isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {

		// Unhides cursor since camera is not looking around anymore
		input->setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	return isPerspective ? 1 : (distance);
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
	float halfHeight = orthoSize;
	float halfWidth = halfHeight * getAspectRatio(); 

	projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
	return;
}
