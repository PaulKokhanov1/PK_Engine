#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// Texture Coordinates from Vertex Shader
in vec2 texCoord;

uniform sampler2D screenTexture;

void main() {
	
	vec4 teapotColor = texture(screenTexture, texCoord);
	vec3 backgroundColor = vec3(1.0f,0.0f,0.0f);
	vec3 color = mix(backgroundColor, teapotColor.rgb, teapotColor.a);
	FragColor = vec4(color, 1.0f);
}