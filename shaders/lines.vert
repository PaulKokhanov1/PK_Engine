#version 460 core

// Positions/Coordinates in model view
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

out DATA
{
	vec3 normal;			
	vec2 texCoord;			// Texture Coordiantes for Fragment Shader
} data_out;

void main()
{

	// Keep in object space as Tessellation will introduce new triangles
	gl_Position = vec4(aPos, 1.0f);

	// Sending transformed normals to be used as color data in World Space
	data_out.normal = aNormal;
	data_out.texCoord = aTex;
}