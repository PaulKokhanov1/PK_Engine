#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// Normals from Vertex Shader
in vec3 Normal;
// Current position from Vertex Shader
in vec3 crntPos;

void main()
{
	vec3 normal = normalize(Normal);

	// Change color of object depending on direction normal is pointing
	FragColor = vec4(abs(normal), 1.0f);
}