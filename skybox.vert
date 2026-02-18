#version 330 core

// Positions/Coordinates in Clip space
layout (location = 0) in vec3 aPos;

// Direction camera is looking towards vertex in World Space
out vec3 dir;

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 camPosWorld;

void main()
{
	// Outputs the positions in clip space
	gl_Position = vec4(aPos, 1.0f);

	// Convert Vertex pos to WS then compute direction from camera to vertex
	mat4 InvProjView = inverse(projectionMatrix * viewMatrix);
	vec4 clipPos = vec4(aPos, 1.0f);
	vec4 WSPos4 = InvProjView * clipPos;
	vec3 WSPos = vec3(WSPos4.xyz / WSPos4.w);
	dir = normalize(WSPos - camPosWorld);

}