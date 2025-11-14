#version 330 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;

// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs normals for Fragment Shader
out vec3 Normal;

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
   	// Calculates current position
	crntPos = vec3(modelMatrix * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = projectionMatrix * viewMatrix * vec4(crntPos, 1.0);

	// Sending transformed normals to be used as color data
	Normal = mat3(transpose(inverse(viewMatrix * modelMatrix))) * aNormal;


}