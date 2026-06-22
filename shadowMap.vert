#version 330 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;

// Transformation Matrices
uniform mat4 matrixShadow;
uniform mat4 modelMatrix;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = matrixShadow * modelMatrix * vec4(aPos, 1.0);
}