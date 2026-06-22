#version 330 core

// Current fragment position in World Space
in vec4 crntPosWrld;

uniform vec3 lightPosWorld;
uniform float farPlane;

void main() {
	gl_FragDepth = length(crntPosWrld.xyz - lightPosWorld) / farPlane;
}