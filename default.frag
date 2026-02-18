#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// Normals in View Space
in vec3 NormalView;
// Normals in World Space
in vec3 NormalWrld;
// Current fragment position in View Space
in vec3 crntPosView;
// Current fragment position in World Space
in vec3 crntPosWrld;

// Light Position in View Space
in vec3 lightPosView;
// Texture Coordinates from Vertex Shader
in vec2 texCoord;


// Camera Position World Space
uniform vec3 camPosWorld;

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
uniform sampler2D texDiffuse;
uniform sampler2D texAmbient;
uniform sampler2D texSpecular;

// Environment Map
uniform samplerCube env;


vec4 pointLight()
{	

	vec3 ambient = texture(texAmbient, texCoord).rgb * Ka * lightKa;

	// Find light direction to current position
	vec3 lightDir = normalize(lightPosView - crntPosView);
	
	// Calculate diffuse shading
	vec3 normal = normalize(NormalView);
	float diffuseAmount = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = texture(texDiffuse, texCoord).rgb * lightKd * (lightColor * Kd * diffuseAmount);

	// Calculate Specular lighting
	vec3 viewDir = normalize(-crntPosView);  // View dir because camera is at origin in View space
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(normal, halfVector), 0.0), shininess);
	vec3 specular = texture(texSpecular, texCoord).r * lightKs * (lightColor * Ks * specAmount);

	return vec4(diffuse + ambient + specular, 1.0);
}

vec4 envReflection()
{
	vec3 normal = normalize(NormalWrld);
	vec3 w_o = normalize(camPosWorld - crntPosWrld);  // CubeMap sampled in World Space since its statically generated
	vec3 w_r = reflect(-w_o , normal);	// direction towards light

	float Lenv = 0.8f;	// Intensity of environment light
	vec3 envColor = texture(env, w_r).rgb;

	vec3 envLight = Lenv * envColor * lightKs;
	return vec4(envLight, 1.0f);
}

vec4 normalLight() {
	vec3 normal = normalize(NormalView);

	// Change color of object depending on direction normal is pointing
	return vec4(abs(normal), 1.0f);
}

void main()
{
	FragColor = mix(envReflection(), pointLight(), 0.6);
	//FragColor = pointLight();
}