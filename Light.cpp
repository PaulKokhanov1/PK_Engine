#include "Light.h"

bool Light::validate()
{
	// Will set Dirty to true regardless of outcome FOR NOW
	dirty = false;

	// Validate Point Light Specific elements
	if (!IsFiniteVec3(position) || !IsFiniteVec3(color)) {
		LogLightError("Light Values are NOT finite");

		// Set position to origin to visually indicate issue
		position = glm::vec3(0.f);
		color = glm::vec3(0.f);

		return false;
	}

	if (glm::any(glm::lessThan(ambient, glm::vec3(0.f))) || glm::any(glm::lessThan(diffuse, glm::vec3(0.f))) || glm::any(glm::lessThan(specular, glm::vec3(0.f)))) {
		LogLightWarn("Invalid Light intensities");

		// Set ALL to 0 intensity to visually indicate issue
		ambient = glm::vec3(0.f);
		diffuse = glm::vec3(0.f);
		specular = glm::vec3(0.f);

		return false;
	}


	return true;
}

void Light::sendLightDataToShader(Shader& shader)
{
	if (dirty) validate();
	shader.setUniform1i("lightType", static_cast<int>(lightType));
	shader.setUniform3fv("lightColor", color);
	shader.setUniform3fv("lightKa", ambient);
	shader.setUniform3fv("lightKd", diffuse);
	shader.setUniform3fv("lightKs", specular);
	shader.setUniform3fv("lightPosWorld", position);
	shader.setUniform3fv("lightDirection", direction);
	shader.setUniform1f("envLightIntensity", envLightIntensity);
	shader.setUniform1f("innerCone", innerCone);
	shader.setUniform1f("outerCone", outerCone);
}

void Light::setlightMesh(std::unique_ptr<MeshComponent> lM)
{
	if (lM) lightMesh = std::move(lM);
}

MeshComponent* Light::getMesh()
{
	if (lightMesh) return lightMesh.get();
	else {
		LogLightError("Light Mesh does not exist");
		return nullptr;
	}
}
