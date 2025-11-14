#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<vector>

#include"EngineConfig.h"
#include"Shader.h"
#include"InputManager.h"

class Camera
{
public:

	// Transformation Matrices for Vertex Shader
	glm::mat4 view = glm::mat4(1.0f);
	mutable glm::mat4 projection = glm::mat4(1.0f);
	mutable bool projectionDirty = true; // Avoid recomputing projection every frame, mutuable to be editable inside a const function

	int width;
	int height;
	float FOV;
	float nearPlane;
	float farPlane;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 1.0f;
	float sensitivity = 1.0f;

	// Handle Camera rotation
	float yaw = -90.0f;
	float pitch = 0.0f;

	// Camera constructor to set up initial values
	Camera(int w, int h, glm::vec3 pos, float FOV, float nearP, float farP);

	// Updates the camera matrix to the Vertex Shader
	void updateViewProjection();
	// Exports View and Projection Matrix to a shader
	void sendToShader(Shader& shader);
	// Handles camera inputs
	void updateInputs(InputManager& input, float dt);

	// Getters
	const glm::vec3& getPosition() const;
	const glm::vec3& getOrientation() const;
	const glm::mat4 getProjectionMatrix() const;
	const glm::mat4 getViewMatrix() const;
	float getDistanceScale() const; // Used for Orthographic projection
	inline float getAspectRatio() const;

	// Setters
	void setFOV(float newFOV);
	void setClipPlanes(float nearP, float farP);


private:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);	// Direction of Camera
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Orthographic and perspective projection toggle
	bool isPerspective = true;
	float referenceDistance = 0.0f;

	void recomputeProjection() const;

};

#endif // !CAMERA_CLASS_H
