#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// direction to determine which cube map face to sample
in vec3 dir;

uniform samplerCube env;

void main() {

	FragColor = texture(env, dir);
}