#version 330 core

// Output Colors in RGBA
out vec4 FragColor;

// Normals in World Space
in vec3 NormalWrld;
// Current fragment position in World Space
in vec3 crntPosWrld;
// Texture Coordinates from Vertex Shader
in vec2 texCoord;

// Camera Position World Space
uniform vec3 camPosWorld;

// Light Attributes
uniform int lightType;	// Point = 0, Directional = 1, Spot = 2
uniform vec3 lightColor;
uniform vec3 lightKa;	// Intensities of ambient, diffuse and specular
uniform vec3 lightKd;
uniform vec3 lightKs;
uniform vec3 lightPosWorld;		// Used for point and spot light
uniform vec3 lightDirection;	// Used for Directional and spotlight
uniform float innerCone;		// Used for spotlight
uniform float outerCone;

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

// Environment Lighting
uniform float envLightIntensity;


vec4 pointLight()
{	
	vec3 lightVec = lightPosWorld - crntPosWrld;

	// Inversely Quadratic formula to lower light intensity as we go further away from light source
	float dist = length(lightVec);
	float a = 1.0f;
	float b = 0.7f;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	vec3 ambient = texture(texAmbient, texCoord).rgb * Ka * lightKa;

	// Find light direction to current position
	vec3 lightDir = normalize(lightVec);
	
	// Calculate diffuse shading
	vec3 normal = normalize(NormalWrld);
	float diffuseAmount = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = texture(texDiffuse, texCoord).rgb * lightKd * (lightColor * Kd * diffuseAmount * intensity);

	// Calculate Specular lighting
	vec3 viewDir = normalize(camPosWorld - crntPosWrld);  // View dir vector pointing at Camera
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(normal, halfVector), 0.0), shininess);
	vec3 specular = texture(texSpecular, texCoord).r * lightKs * (lightColor * Ks * specAmount * intensity);

	return vec4(diffuse + ambient + specular, 1.0);
}

vec4 directionalLight()
{	
	vec3 ambient = texture(texAmbient, texCoord).rgb * Ka * lightKa;

	// Find direction TOWARDS light source
	vec3 lightDir = normalize(-lightDirection);
	
	// Calculate diffuse shading
	vec3 normal = normalize(NormalWrld);
	float diffuseAmount = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = texture(texDiffuse, texCoord).rgb * lightKd * (lightColor * Kd * diffuseAmount);

	// Calculate Specular lighting
	vec3 viewDir = normalize(camPosWorld - crntPosWrld);  // View dir vector pointing at Camera
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(normal, halfVector), 0.0), shininess);
	vec3 specular = texture(texSpecular, texCoord).r * lightKs * (lightColor * Ks * specAmount);

	return vec4(diffuse + ambient + specular, 1.0);
}

vec4 spotLight()
{	
	vec3 lightVec = lightPosWorld - crntPosWrld;

	vec3 ambient = texture(texAmbient, texCoord).rgb * Ka * lightKa;

	// Find light direction to current position
	vec3 lightDir = normalize(lightVec);

	// Determine cutoff angle
	float angle = dot(lightDir , normalize(-lightDirection));
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	
	// Calculate diffuse shading
	vec3 normal = normalize(NormalWrld);
	float diffuseAmount = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = texture(texDiffuse, texCoord).rgb * lightKd * (lightColor * Kd * diffuseAmount * inten);

	// Calculate Specular lighting
	vec3 viewDir = normalize(camPosWorld - crntPosWrld);  // View dir vector pointing at Camera
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(normal, halfVector), 0.0), shininess);
	vec3 specular = texture(texSpecular, texCoord).r * lightKs * (lightColor * Ks * specAmount * inten);

	return vec4(diffuse + ambient + specular, 1.0);
}

vec4 envReflection()
{
	vec3 normal = normalize(NormalWrld);
	vec3 w_o = normalize(camPosWorld - crntPosWrld);  // CubeMap sampled in World Space since its statically generated
	vec3 w_r = reflect(-w_o , normal);	// direction towards light

	vec3 envColor = texture(env, w_r).rgb;

	vec3 envLight = envLightIntensity * envColor * lightKs;
	return vec4(envLight, 1.0f);
}

vec4 normalLight() {
	vec3 normal = normalize(NormalWrld);

	// Change color of object depending on direction normal is pointing
	return vec4(abs(normal), 1.0f);
}

void main()
{
	if (lightType == 0) {
		FragColor = pointLight();
	} else if (lightType == 1) {
		FragColor = directionalLight();
	} else if (lightType == 2) {
		FragColor = spotLight();
	}
	//FragColor = envReflection();
	vec3 finalColor = pointLight().rgb + envReflection().rgb * 0.6;
	//FragColor = vec4(finalColor,1.0);
}