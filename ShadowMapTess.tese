#version 460 core

layout(quads, equal_spacing, ccw) in;

in DATA
{
	vec3 normal;
	vec2 texCoord;
} data_in[];


out vec3 worldPos;


// Transformation Matrices
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 matrixShadow;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

// Texture Units
uniform sampler2D texDisplacement;

// Render Settings
uniform float displacementScale;

struct SurfacePoint
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

SurfacePoint EvaluateSurface(vec2 uvCoord)
{
    SurfacePoint s;

	// Perform barycentric interpolation
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

	SurfacePoint P  = EvaluateSurface(gl_TessCoord.xy);
	worldPos = vec3(modelMatrix * vec4(P.position, 1.0));
	return matrixShadow * modelMatrix * vec4(P.position,1);
}

void main() {

	gl_Position = quads();
}
