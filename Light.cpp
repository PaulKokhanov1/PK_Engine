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

void Light::recomputeLightProjection() const
{
	// create Projection matrix for perspective projection
	if (isPerspective) {
		mLightProjection = glm::perspective(glm::radians(FOV), getAspectRatio(), nearPlane, farPlane);
		return;
	}

	// Orthographic projection only used for directional lights
	// Multiply by aspectRatio to have equal proportions
	float halfHeight = directionalShadowExtent;
	float halfWidth = halfHeight * getAspectRatio();

	mLightProjection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
	return;
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
	shader.setUniform3fv("lightDirection", rotation * constants::default_forward);
	shader.setUniform1f("innerCone", innerCone);
	shader.setUniform1f("outerCone", outerCone);
	shader.setUniform1f("farPlane", farPlane);
}

void Light::sendLightFarPlaneToShader(Shader& shader)
{
	if (dirty) validate();

	shader.setUniform1f("farPlane", farPlane);
}

void Light::sendLightLightPosToShader(Shader& shader)
{
	if (dirty) validate();

	shader.setUniform3fv("lightPosWorld", position);
}

void Light::sendShadowMatrices(Shader& shader)
{
	updateShadowMatrices();
	if (mShadowMap->getShadowMapType() == ShadowMapType::DEPTH_TEXTURE) shader.setUniformMat4fv("matrixShadow", mLightProjection * mLightViews[0]);
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

const glm::mat4& Light::getLightProjectionMatrix() const
{
	if (mlightProjectionDirty) {
		recomputeLightProjection();
		mlightProjectionDirty = false;
	}
	return mLightProjection;
}

const std::array<glm::mat4, 6>& Light::getLightViewMatrices()
{
	updateShadowMatrices();

	return mLightViews;
}

inline float Light::getAspectRatio() const
{
	auto [w, h] = mShadowMap->getDepthTextureDimensions();
	return static_cast<float>(w) / std::max(1, h);
}

void Light::updateShadowMatrices()
{
	if (!mShadowMatricesDirty) {
		return;
	}

	switch (lightType)
	{
		case LightType::POINT:
			mLightViews[0] = glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
			mLightViews[1] = glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
			mLightViews[2] = glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
			mLightViews[3] = glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
			mLightViews[4] = glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
			mLightViews[5] = glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
			break;
		case LightType::DIRECTIONAL:
			glm::vec3 shadowPos = -(rotation * constants::default_forward) * directionalShadowDistance;	// Creat a fake position for a directional light
			mLightViews[0] = glm::lookAt(shadowPos, shadowPos + (rotation * constants::default_forward), Up);	// look one unit farther along light direction
			break;
		case LightType::SPOT:
			mLightViews[0] = glm::lookAt(position, position + (rotation * constants::default_forward), Up); // Use direction of looking, derived from rotation
			break;

	}

	recomputeLightProjection();
	mShadowMatricesDirty = false;
}
