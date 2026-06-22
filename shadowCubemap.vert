#version 330 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;

out vec4 crntPosWrld;

// Transformation Matrices
uniform mat4 matrixShadow;
uniform mat4 modelMatrix;

void main()
{
	crntPosWrld = modelMatrix * vec4(aPos, 1.0f);
	// Outputs the positions/coordinates of all vertices
	gl_Position = matrixShadow * crntPosWrld;
}