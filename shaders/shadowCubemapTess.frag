#version 460 core

// Current fragment position in World Space
in vec3 crntPosWrld;

uniform vec3 lightPosWorld;
uniform float farPlane;

void main() {
	gl_FragDepth = length(crntPosWrld - lightPosWorld) / farPlane;
}