#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// Normals from Vertex Shader
in vec3 Normal;
// Current position from Vertex Shader
in vec3 crntPos;
// Current fragment position in ViewSpace
in vec3 crntPosView;
// Light Position in View Space
in vec3 lightPosView;


// Light Attributes
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 lightKa;	// Intensities of ambient, diffuse and specular
uniform vec3 lightKd;
uniform vec3 lightKs;

// Material Attributes
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;


vec4 pointLight()
{	

	vec3 ambient = Ka * lightKa;

	// Find light direction to current position
	vec3 lightDir = normalize(lightPosView - crntPosView);
	
	// Calculate diffuse shading
	vec3 normal = normalize(Normal);
	float diffuseAmount = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = lightKd * (lightColor * Kd * diffuseAmount);

	// Calculate Specular lighting
	vec3 viewDir = normalize(-crntPosView);
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(normal, halfVector), 0.0), shininess);
	vec3 specular = lightKs * (lightColor * Kd * specAmount);

	return vec4(diffuse + ambient + specular, 1.0);
}

void main()
{
	vec3 normal = normalize(Normal);

	// Change color of object depending on direction normal is pointing
	// FragColor = vec4(abs(normal), 1.0f);

	FragColor = pointLight();
}