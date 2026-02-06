#version 330 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

// Texture Coordiantes for Fragement Shader
out vec2 texCoord;

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);

	// Sending texture coordinates
	texCoord = aTex;

}