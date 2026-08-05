#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in DATA
{
	vec4 lightView_Position_ClipSpace;
	vec3 Normal_WorldSpace;
	vec2 texCoord;
} data_in[];

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;


void main() 
{
	modelMatrix * gl_in[0].gl_Position;

    gl_Position = projectionMatrix * ((viewMatrix * modelMatrix * gl_in[0].gl_Position) +  0.01f * vec4(data_in[0].Normal_WorldSpace, 0.0f));
    EmitVertex();    
	gl_Position = projectionMatrix * ((viewMatrix * modelMatrix * gl_in[1].gl_Position) +  0.01f * vec4(data_in[1].Normal_WorldSpace, 0.0f));
    EmitVertex();  
	gl_Position = projectionMatrix * ((viewMatrix * modelMatrix * gl_in[2].gl_Position) +  0.01f * vec4(data_in[2].Normal_WorldSpace, 0.0f));
    EmitVertex();	
	gl_Position = projectionMatrix * ((viewMatrix * modelMatrix * gl_in[0].gl_Position) +  0.01f * vec4(data_in[0].Normal_WorldSpace, 0.0f));
    EmitVertex();
	EndPrimitive();
}