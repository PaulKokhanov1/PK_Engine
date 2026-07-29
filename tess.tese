#version 460 core

layout(quads, equal_spacing, ccw) in;

in DATA
{
	vec3 normal;
	vec2 texCoord;
} data_in[];

out DATA
{
	vec4 lightView_Position_ClipSpace;	// Outputs the current position in light view space for the Fragment Shader for Shadows
	vec3 Normal_WorldSpace;		// Outputs normals in World Space for Fragment Shader
	vec2 texCoord;				// Texture Coordiantes for Fragment Shader

} data_out;

// Transformation Matrices
uniform mat3 normalMatrix;
uniform mat4 matrixShadow;
uniform mat4 modelMatrix;

// Texture Units
uniform sampler2D texDisplacement;

// Render Settings
uniform float displacementScale;

struct EvaluatedVertex
{
    vec3 position;
    vec3 normal;
    vec2 uv;
};


vec4 interpolateVec4( vec4 v0, vec4 v1, vec4 v2, vec4 v3, vec2 uv) {

	vec4 a = mix(v0, v1, uv.x);
	vec4 b = mix(v3, v2, uv.x);
	return mix(a, b, uv.y);
}

vec3 interpolateVec3( vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec2 uv) {

	vec3 a = mix(v0, v1, uv.x);
	vec3 b = mix(v3, v2, uv.x);
	return mix(a, b, uv.y);
}

vec2 interpolateVec2( vec2 v0, vec2 v1, vec2 v2, vec2 v3, vec2 uv) {

	vec2 a = mix(v0, v1, uv.x);
	vec2 b = mix(v3, v2, uv.x);
	return mix(a, b, uv.y);
}

EvaluatedVertex EvaluateSurface(vec2 uvCoord)
{
    EvaluatedVertex s;

	vec4 pos = interpolateVec4( gl_in[0].gl_Position, gl_in[1].gl_Position,gl_in[2].gl_Position, gl_in[3].gl_Position, uvCoord);
	vec3 normal = normalize(interpolateVec3( data_in[0].normal, data_in[1].normal, data_in[2].normal, data_in[3].normal, uvCoord));
	vec2 UV = interpolateVec2( data_in[0].texCoord, data_in[1].texCoord, data_in[2].texCoord, data_in[3].texCoord, uvCoord);
	float depth = texture(texDisplacement, UV).r;

	pos.xyz += normal * depth * displacementScale;

    s.position = pos.xyz;
    s.normal   = normal;
    s.uv       = UV;

    return s;

}

vec4 quads() {

	vec2 uv = gl_TessCoord.xy;

	EvaluatedVertex P  = EvaluateSurface(uv);

	data_out.lightView_Position_ClipSpace = matrixShadow * modelMatrix * vec4(P.position,1);
	data_out.Normal_WorldSpace = normalize(normalMatrix * P.normal);
    data_out.texCoord = P.uv;

	return vec4(P.position,1);
}

vec4 triangles() {

	// TessCoord is OpenGl's built in 2D tessellation coordinate, which for a triangle output is stored as barycentric coordinates
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float w = gl_TessCoord.z;

	// Perform barycentric interpolation
	vec4 pos = gl_in[0].gl_Position * u + gl_in[1].gl_Position * v + gl_in[2].gl_Position * w;
	vec3 normal = data_in[0].normal * u + data_in[1].normal * v + data_in[2].normal * w;
	normal = normalize(normal);
	vec2 UV = u * data_in[0].texCoord + v * data_in[1].texCoord + w * data_in[2].texCoord;

	float depth = texture(texDisplacement, UV).r;

	pos.xyz += normal * depth * displacementScale;

	data_out.lightView_Position_ClipSpace = matrixShadow *  modelMatrix * pos;
	data_out.Normal_WorldSpace = normalize(normalMatrix * normal);
    data_out.texCoord = UV;

	return pos;
}

void main() {

	gl_Position = quads();
}
