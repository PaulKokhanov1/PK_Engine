#version 330 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

// Sends Position in CVV/Clip space to FS
out vec4 positionCVV;

// Sends texture coordinates
out vec2 meshUV;

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	meshUV = aTex;

	// Outputs the positions/coordinates of all vertices
	positionCVV = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
	gl_Position = positionCVV;

}