#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// Position in CVV/Clip space
in vec4 positionCVV;

// Texture Coordinates
in vec2 meshUV;

uniform sampler2D screenTexture;

vec4 projectiveTextureMapping() {
	// Calculate projective texture mapping
	vec2 projectiveTexCoord = positionCVV.xy / positionCVV.w; // convert coord's to NDC
	vec2 uv = projectiveTexCoord.xy * 0.5 + 0.5;	// map from [-1,1] -> [0,1]
	vec4 teapotColor = texture(screenTexture, uv);
	vec3 backgroundColor = vec3(1.0f,0.0f,0.0f);
	vec3 color = mix(backgroundColor, teapotColor.rgb, teapotColor.a);
	return vec4(color, 1.0f);
}

vec4 quadLocalSurfaceTextureMapping() {
	
	vec4 teapotColor = texture(screenTexture, meshUV);
	vec3 backgroundColor = vec3(1.0f,0.0f,0.0f);
	vec3 color = mix(backgroundColor, teapotColor.rgb, teapotColor.a);
	return vec4(color, 1.0f);
}

void main() {
	FragColor = quadLocalSurfaceTextureMapping();
}