#version 460 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

// Outputs the current position in world space for the Fragment Shader
out vec3 crntPos_WorldSpace;
// Outputs normals in World Space for Fragment Shader
out vec3 Normal_WorldSpace;
// Texture Coordiantes for Fragment Shader
out vec2 texCoord;
// Outputs the current position in light view space for the Fragment Shader for Shadows
out vec4 lightView_Position_ClipSpace;

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 matrixShadow;

void main()
{
	// convert current fragment position to world space
	crntPos_WorldSpace = vec3(modelMatrix * vec4(aPos, 1.0f));
	// convert current fragment position to light view space
	lightView_Position_ClipSpace = matrixShadow * modelMatrix * vec4(aPos, 1.0f);
	// Outputs the positions/coordinates of all vertices
	gl_Position = projectionMatrix * viewMatrix * vec4(crntPos_WorldSpace, 1.0);

	// Sending transformed normals to be used as color data in World Space
	Normal_WorldSpace = normalize(normalMatrix * aNormal);

	// Sending texture coordinates
	texCoord = aTex;

}