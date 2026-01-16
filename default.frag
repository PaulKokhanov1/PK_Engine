#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// Normals from Vertex Shader
in vec3 Normal;
// Current fragment position in ViewSpace
in vec3 crntPosView;
// Light Position in View Space
in vec3 lightPosView;
// Texture Coordinates from Vertex Shader
in vec2 texCoord;


// Light Attributes
uniform vec3 lightColor;
uniform vec3 lightKa;	// Intensities of ambient, diffuse and specular
uniform vec3 lightKd;
uniform vec3 lightKs;

// Material Attributes
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;

// Texture Units
uniform sampler2D texDiff;
uniform sampler2D texAmb;
uniform sampler2D texSpec;


vec4 pointLight()
{	

	vec3 ambient =  texture(texAmb, texCoord).rgb * Ka * lightKa;

	// Find light direction to current position
	vec3 lightDir = normalize(lightPosView - crntPosView);
	
	// Calculate diffuse shading
	vec3 normal = normalize(Normal);
	float diffuseAmount = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = texture(texDiff, texCoord).rgb * lightKd * (lightColor * Kd * diffuseAmount);

	// Calculate Specular lighting
	vec3 viewDir = normalize(-crntPosView);
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(normal, halfVector), 0.0), shininess);
	vec3 specular = texture(texSpec, texCoord).r * lightKs * (lightColor * Kd * specAmount);

	return vec4(diffuse + ambient + specular, 1.0);
}

vec4 normalLight() {
	vec3 normal = normalize(Normal);

	// Change color of object depending on direction normal is pointing
	return vec4(abs(normal), 1.0f);
}

void main()
{

	FragColor = pointLight();
}