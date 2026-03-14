#version 330 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

// Outputs the current position in world space for the Fragment Shader
out vec3 crntPosWrld;
// Outputs normals in World Space for Fragment Shader
out vec3 NormalWrld;
// Used for storing projected reflection texture position
out vec4 reflectionPosition;

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 reflectionViewMatrix;
uniform mat3 normalMatrix;

void main()
{
	// convert current fragment position to world space
	crntPosWrld = vec3(modelMatrix * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = projectionMatrix * viewMatrix * vec4(crntPosWrld, 1.0);

	// Sending transformed normals to be used as color data in World Space
	NormalWrld = normalMatrix * aNormal;

	// Sending reflected texture positions
	mat4 reflectProjectWorld = projectionMatrix * reflectionViewMatrix * modelMatrix;

	reflectionPosition = reflectProjectWorld * vec4(aPos, 1.0f);

}