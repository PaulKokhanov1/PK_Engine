#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec4 lightView_Position_ClipSpace;
out vec3 lightPos_TangentSpace;
out vec3 lightDirection_TangentSpace;
out vec3 camPos_TangentSpace;
out vec3 Normal_WorldSpace;
out vec3 crntPos_TangentSpace;
out vec3 lightPos_WorldSpace;
out vec3 lightDirection_WorldSpace;		
out vec3 crntPos_WorldSpace;		
out vec3 camPos_WorldSpace;		
out vec2 texCoord;

in DATA
{
	vec4 lightView_Position_ClipSpace;
	vec3 Normal_WorldSpace;
	vec2 texCoord;

} data_in[];

// Lighting unifroms to be transformed to tangent space
uniform vec3 camPosWorld;		// Camera Position World Space
uniform vec3 lightPosWorld;		// Used for point and spot light
uniform vec3 lightDirection;	// Used for Directional and spotlight

// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;


void main()
{

    // gk_position in OBJECT SPACE from TES, use WORLD SPACE pos instead for calc
    vec3 deltaPos1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 deltaPos2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec2 deltaUV1 = data_in[1].texCoord - data_in[0].texCoord;
    vec2 deltaUV2 = data_in[2].texCoord - data_in[0].texCoord;

    float invDet = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

    vec3 tangent = vec3(invDet * (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y));
    //vec3 bitangent = vec3(invDet * (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x));

    vec3 normal_worldspace = normalize(data_in[0].Normal_WorldSpace + data_in[1].Normal_WorldSpace + data_in[2].Normal_WorldSpace);
    vec3 tangent_worldspace = vec3(mat3(modelMatrix) * tangent);  // potentially need to use normalMatrix here aswell? not sure
    tangent_worldspace = tangent_worldspace - dot(tangent_worldspace, normal_worldspace) * normal_worldspace;
    vec3 bitangent_worldspace = -cross(normal_worldspace, tangent_worldspace);


    for (int i = 0; i < 3; ++i) {
    
        // Compute TBN PER VERTEX
        vec3 N = normalize(data_in[i].Normal_WorldSpace);

        vec3 T = tangent_worldspace;
        T = normalize(T - dot(T, N) * N);

        vec3 B = cross(N, T);
        mat3 TBN = transpose(mat3(T,B,N));

        vec3 worldPos = vec3(modelMatrix * gl_in[i].gl_Position);

        gl_Position = projectionMatrix * viewMatrix * vec4(worldPos, 1.0f);
        texCoord = data_in[i].texCoord;
        crntPos_TangentSpace = TBN * worldPos;
        camPos_TangentSpace = TBN * camPosWorld;
        lightDirection_TangentSpace = TBN * lightDirection;
        lightPos_TangentSpace = TBN * lightPosWorld;
        crntPos_WorldSpace = worldPos;
        lightDirection_WorldSpace = lightDirection;
        lightPos_WorldSpace = lightPosWorld;
        camPos_WorldSpace = camPosWorld;
        Normal_WorldSpace = normalize(data_in[i].Normal_WorldSpace);
        lightView_Position_ClipSpace = data_in[i].lightView_Position_ClipSpace;
        EmitVertex();
    }

    EndPrimitive();
}