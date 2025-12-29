#version 330 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;

// Outputs the current position in view space for the Fragment Shader
out vec3 crntPosView;
// Outputs normals for Fragment Shader
out vec3 Normal;
// Outputs light position in view space
out vec3 lightPosView;

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPosWorld;

void main()
{
   	// Calculates current fragment position in world space
	vec3 crntPos = vec3(modelMatrix * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = projectionMatrix * viewMatrix * vec4(crntPos, 1.0);

	// Convert light position to view space
	lightPosView = vec3(viewMatrix * vec4(lightPosWorld, 1.0));
	// Convert current position to view space
	crntPosView = vec3(viewMatrix * modelMatrix * vec4(aPos, 1.0f));

	// Sending transformed normals to be used as color data
	Normal = mat3(transpose(inverse(viewMatrix * modelMatrix))) * aNormal;


}