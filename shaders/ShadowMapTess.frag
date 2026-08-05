#version 460 core

// Output Colors in RGBA
out vec4 FragColor;

// Current fragment position in World Space
in vec3 worldPos;

void main() {
	
	FragColor = vec4(worldPos * 0.5 + 0.5, 1.0);
}