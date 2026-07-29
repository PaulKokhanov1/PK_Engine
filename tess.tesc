#version 460 core

layout (vertices = 4) out; // REMEMEBER TO CHANGE THIS FOR QUADS

in DATA
{
	vec3 normal;
	vec2 texCoord;
} data_in[];

out DATA
{
	vec3 normal;			// Outputs normals in Object Space
	vec2 texCoord;			// Texture Coordiantes for Fragment Shader
} data_out[];

// Render Settings
uniform float tessellationLevel;

void quads()
{
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = tessellationLevel; // left for quads
        gl_TessLevelOuter[1] = tessellationLevel; // bot for quads
        gl_TessLevelOuter[2] = tessellationLevel; // right for quads
        gl_TessLevelOuter[3] = tessellationLevel; // top for quads
        
        gl_TessLevelInner[0] = tessellationLevel;
        gl_TessLevelInner[1] = tessellationLevel;
    }

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	data_out[gl_InvocationID].normal = data_in[gl_InvocationID].normal;
    data_out[gl_InvocationID].texCoord = data_in[gl_InvocationID].texCoord;
}

void triangles()
{
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = tessellationLevel;
        gl_TessLevelOuter[1] = tessellationLevel;
        gl_TessLevelOuter[2] = tessellationLevel;
        
        gl_TessLevelInner[0] = 2 * tessellationLevel;
    }

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	data_out[gl_InvocationID].normal = data_in[gl_InvocationID].normal;
    data_out[gl_InvocationID].texCoord = data_in[gl_InvocationID].texCoord;
}

void main() {

    quads();
}