#version 460 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

out vec4 crntPosWrld;

out DATA
{
	vec3 normal;			
	vec2 texCoord;			// Texture Coordiantes for Fragment Shader
} data_out;

// Transformation Matrices
uniform mat4 matrixShadow;
uniform mat4 modelMatrix;

void main()
{

	// Sending transformed normals to be used as color data in World Space
	data_out.normal = aNormal;
	data_out.texCoord = aTex;

	crntPosWrld = modelMatrix * vec4(aPos, 1.0f);
	// Outputs the positions/coordinates of all vertices
	gl_Position = matrixShadow * crntPosWrld;
}