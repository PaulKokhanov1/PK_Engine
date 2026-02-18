#version 330 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

// Outputs the current position in view space for the Fragment Shader
out vec3 crntPosView;
// Outputs the current position in world space for the Fragment Shader
out vec3 crntPosWrld;
// Outputs normals in View Space for Fragment Shader
out vec3 NormalView;
// Outputs normals in World Space for Fragment Shader
out vec3 NormalWrld;
// Outputs light position in view space
out vec3 lightPosView;
// Texture Coordiantes for Fragement Shader
out vec2 texCoord;

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform float camDistanceScale;
uniform vec3 lightPosWorld;

void main()
{
	// convert current fragment position to world space
	crntPosWrld = vec3(modelMatrix * camDistanceScale * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = projectionMatrix * viewMatrix * vec4(crntPosWrld, 1.0);

	// Convert light position to view space
	lightPosView = vec3(viewMatrix * vec4(lightPosWorld, 1.0));
	// Convert current position to view space
	crntPosView = vec3(viewMatrix * modelMatrix * vec4(aPos, 1.0f));

	// Sending transformed normals to be used as color data in View Space
	NormalView = mat3(transpose(inverse(viewMatrix * modelMatrix))) * aNormal;
	
	// Sending transformed normals to be used as color data in World Space
	NormalWrld = mat3(transpose(inverse(modelMatrix))) * aNormal;

	// Sending texture coordinates
	texCoord = aTex;

}