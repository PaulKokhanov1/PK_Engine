#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<vector>

#include"LogCamera.h"
#include"EngineConfig.h"
#include"Shader.h"
#include"Application.h"
#include"InputManager.h"

class Camera
{
public:

	// Transformation Matrices for Vertex Shader
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 mirroredView = glm::mat4(1.0f);
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
	// Calculate Mirrored View Matrix for computing reflections on a plane given by its Normal and center pos
	void calcMirroredViewMatrix(glm::vec3 mirrorNormal, glm::vec3 mirrorPos);
	// Exports View and Projection Matrix to a shader
	void sendViewAndProjToShader(Shader& shader);	
	// Exports MIRROED View and Projection Matrix to a shader
	void sendMirroredViewAndProjToShader(Shader& shader);
	// Export Mirrored View to a shader
	void sendMirroredViewToShader(Shader& shader);
	// Export Inverse Projection View Matrix
	void sendInverseProjViewToShader(Shader& shader);
	// Exports Camera World Space Pos to a shader
	void sendCamPositionWorldSpaceToShader(Shader& shader);	
	// Handles camera inputs
	void updateInputs(float dt);

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
	void setScreenDimensions(int w, int h);


private:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 mirroredPosition;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);	// Direction of Camera
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Orthographic and perspective projection toggle
	bool isPerspective = true;
	float orthoSize = 0.0f;
	float zoomFactor = 0.2f;

	void recomputeProjection() const;

};

