#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// Current fragment position in World Space
in vec3 crntPosWrld;
// Normals in World Space
in vec3 NormalWrld;
// Projected reflection texture position in CVV
in vec4 reflectionPosition;

// Camera Position World Space
uniform vec3 camPosWorld;

uniform sampler2D screenTexture;

// Environment Map
uniform samplerCube env;

// Environment Lighting
uniform float envLightIntensity;
uniform vec3 lightKs;

vec3 envReflection()
{
	vec3 normal = normalize(NormalWrld);
	vec3 w_o = normalize(camPosWorld - crntPosWrld);  // CubeMap sampled in World Space since its statically generated
	vec3 w_r = reflect(-w_o , normal);	// direction towards light

	vec3 envColor = texture(env, w_r).rgb;

	vec3 envLight = envLightIntensity * envColor * lightKs;
	return envLight;
}

void main() {
	
	// Calculate projective texture mapping
	vec2 reflectTexCoord = reflectionPosition.xy / reflectionPosition.w; // convert coord's to NDC
	vec2 uv = reflectTexCoord.xy * 0.5 + 0.5;	// map from [-1,1] -> [0,1]
	vec4 planeColor = texture(screenTexture, uv);
	vec3 color = mix(envReflection(), planeColor.rgb, planeColor.a);
	FragColor = vec4(color, 1.0f);
}