#version 460 core

// Output Colors in RGBA
out vec4 FragColor;

// Normals in World Space
in vec3 Normal_WorldSpace;
// Current fragment position in World Space

// Texture Coordinates from Vertex Shader
in vec2 texCoord;

in vec4 lightView_Position_ClipSpace;
in vec3 crntPos_TangentSpace;
in vec3 camPos_TangentSpace;			
in vec3 lightPos_TangentSpace;		// Used for point and spot light
in vec3 lightDirection_TangentSpace;		// Used for Directional and spotlight
in vec3 crntPos_WorldSpace;		
in vec3 lightPos_WorldSpace;		
in vec3 lightDirection_WorldSpace;		
in vec3 camPos_WorldSpace;

// Light Attributes
uniform int lightType;	// Point = 0, Directional = 1, Spot = 2
uniform vec3 lightColor;
uniform vec3 lightKa;	// Intensities of ambient, diffuse and specular
uniform vec3 lightKd;
uniform vec3 lightKs;

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
uniform sampler2D texNormal;
uniform sampler2D texDisplacement;


// Transformation Matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

// Environment Map
uniform samplerCube env;

// Shadow Map
uniform sampler2DShadow shadowMap;		// Used for Directional and spotlight
uniform samplerCube	shadowCubeMap;	// Used for Point Lights
uniform float farPlane;				// Used for Point Lights
uniform float shadowCubemapResolution;

// Environment Lighting
uniform float envLightIntensity;

// Render Settings
uniform bool shadowsEnabled;

// Global variables
vec3 Normal_tangentSpace = normalize(texture(texNormal, texCoord).rgb * 2.0f - 1.0f );

float shadowCalculationSampler2D()
{
	if (!shadowsEnabled) 
	{
		return 1.0f;
	}
	
	vec3 lightVec = lightPos_WorldSpace - crntPos_WorldSpace;
	// Find light direction to current position
	vec3 lightDir = normalize(lightVec);

	// Calculate Shadows
	float visibility = 0.0f;
	vec3 lightCoords = lightView_Position_ClipSpace.xyz / lightView_Position_ClipSpace.w;
	if (lightCoords.z <= 1.0f) 
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		float bias = max(0.0008f * (1.0f - dot(Normal_WorldSpace, lightDir)), 0.0006f);

		int sampleRadius = 1;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for (int y = -sampleRadius; y <= sampleRadius; ++y) {
			for (int x = -sampleRadius; x <= sampleRadius; ++x) {

				visibility += texture( shadowMap, vec3 (lightCoords.xy + vec2(x, y) * pixelSize , currentDepth - bias) );
			}
		}
		visibility /= pow((sampleRadius * 2 + 1), 2);
	}
	return visibility;
}

float shadowCalculationSamplerCube()
{
	if (!shadowsEnabled) 
	{
		return 0.0f;	// as currently we use (1.0f - shadow) in color adjusting
	}

	vec3 lightVec = lightPos_WorldSpace - crntPos_WorldSpace;
	// Find light direction to current position
	vec3 lightDir = normalize(lightVec);

	// Shadows
	vec3 fragmentToLight = -lightVec;
	float currentDepth = length(fragmentToLight);
	float shadow = 0.0f;
	float bias = max(0.5f * (1.0f - dot(Normal_WorldSpace, lightDir)), 0.0005f);
	
	// PCF for softer shadows
	int sampleRadius = 1;
	float pixelSize = 1.0f / shadowCubemapResolution;
	for (int z = -sampleRadius; z <= sampleRadius; ++z) {
		for (int y = -sampleRadius; y <= sampleRadius; ++y) {
			for (int x = -sampleRadius; x <= sampleRadius; ++x) {

				float closestDepth = texture(shadowCubeMap, fragmentToLight + vec3(x, y, z) * pixelSize).r;
				closestDepth *= farPlane;	// Put depth into [0,1] range
				if (currentDepth > closestDepth + bias) {
					shadow += 1.0f;
				}
			}
		}
	}
	shadow /= pow((sampleRadius * 2 + 1), 3);

	return shadow;

}

vec4 pointLight()
{	
	vec3 lightVec = lightPos_TangentSpace - crntPos_TangentSpace;
	// Find light direction to current position
	vec3 lightDir = normalize(lightVec);

	// Shadows
	float shadow = shadowCalculationSamplerCube();

	// Inversely Quadratic formula to lower light intensity as we go further away from light source
	float dist = length(lightVec);
	float a = 0.7f;
	float b = 0.1f;
	float intensity = 1.0f / (a * dist * dist + b * dist + 0.3f);

	// Displacement Mapping
	vec3 viewDir = normalize(camPos_TangentSpace - crntPos_TangentSpace);  // View dir vector pointing at Camera

	vec3 ambient = texture(texAmbient, texCoord).rgb * Ka * lightKa;

	// Calculate diffuse shading
	float diffuseAmount = max(dot(Normal_tangentSpace, lightDir), 0.0f);
	vec3 diffuse = texture(texDiffuse, texCoord).rgb * lightKd * (lightColor * Kd * diffuseAmount * intensity * (1.0f - shadow));

	// Calculate Specular lighting
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(Normal_tangentSpace, halfVector), 0.0), shininess);
	vec3 specular = texture(texSpecular, texCoord).r * lightKs * (lightColor * Ks * specAmount * intensity * (1.0f - shadow));

	
	return vec4(diffuse + ambient + specular, 1.0);
}

vec4 directionalLight()
{	
	// Find direction TOWARDS light source
	vec3 lightDir = normalize(-lightDirection_TangentSpace);

	// Calculate Shadows
	float visibility = shadowCalculationSampler2D();

	vec3 viewDir = normalize(camPos_TangentSpace - crntPos_TangentSpace);  // View dir vector pointing at Camera

	vec3 ambient = texture(texAmbient, texCoord).rgb * Ka * lightKa;
	
	// Calculate diffuse shading
	float diffuseAmount = max(dot(Normal_tangentSpace, lightDir), 0.0f);
	vec3 diffuse = texture(texDiffuse, texCoord).rgb * lightKd * (lightColor * Kd * diffuseAmount * visibility);

	// Calculate Specular lighting
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(Normal_tangentSpace, halfVector), 0.0), shininess);
	vec3 specular = texture(texSpecular, texCoord).r * lightKs * (lightColor * Ks * specAmount * visibility);

	return vec4(diffuse + ambient + specular, 1.0);
}

vec4 spotLight()
{	

	// Calculate Shadows
	float visibility = shadowCalculationSampler2D();

	vec3 lightVec = lightPos_TangentSpace - crntPos_TangentSpace;

	vec3 ambient = texture(texAmbient, texCoord).rgb * Ka * lightKa;

	// Find light direction to current position
	vec3 lightDir = normalize(lightVec);

	// Determine cutoff angle
	float angle = dot(lightDir , normalize(-lightDirection_TangentSpace));
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	
	// Calculate diffuse shading
	float diffuseAmount = max(dot(Normal_tangentSpace, lightDir), 0.0f);
	vec3 diffuse = texture(texDiffuse, texCoord).rgb * lightKd * (lightColor * Kd * diffuseAmount * inten * visibility);

	// Calculate Specular lighting
	vec3 viewDir = normalize(camPos_TangentSpace - crntPos_TangentSpace);  // View dir vector pointing at Camera
	vec3 halfVector = normalize(lightDir + viewDir); 
	float specAmount = pow(max(dot(Normal_tangentSpace, halfVector), 0.0), shininess);
	vec3 specular = texture(texSpecular, texCoord).r * lightKs * (lightColor * Ks * specAmount * inten * visibility);

	return vec4(diffuse + ambient + specular, 1.0);
}

vec4 envReflection()
{
	vec3 w_o = normalize(camPos_WorldSpace - crntPos_WorldSpace);  // CubeMap sampled in World Space since its statically generated
	vec3 w_r = reflect(-w_o , Normal_WorldSpace);	// direction towards light

	vec3 envColor = texture(env, w_r).rgb;

	vec3 envLight = envLightIntensity * envColor * lightKs;
	return vec4(envLight, 1.0f);
}

vec4 normalLight() {

	// Change color of object depending on direction normal is pointing
	return vec4(abs(Normal_WorldSpace), 1.0f);
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
	// vec3 finalColor = pointLight().rgb + envReflection().rgb * 0.6;
	//FragColor = vec4(finalColor,1.0);
}