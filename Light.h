#pragma once

#include<glm/glm.hpp>
#include<limits>
#include<array>

#include"LogLight.h"
#include"MathCommon.h"
#include"Shader.h"
#include"MeshComponent.h"
#include"ShadowMap.h"

enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOT
};

struct Light
{
	LightType lightType;

	glm::vec3 color = glm::vec3(0.0f);
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);

	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	// SpotLight Specific
	float innerCone = 0.f;
	float outerCone = 0.f;

	bool shouldShowMesh = false;

	Light() {}

	Light(
		LightType type, glm::vec3 col, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, 
		glm::vec3 pos, 
		float inCone = 0.0f, float outCone = 0.0f, bool showMesh = true) : lightType(type), color(col), ambient(amb), diffuse(diff), specular(spec), position(pos),
		innerCone(inCone), outerCone(outCone),
		shouldShowMesh(showMesh)
	{
		isPerspective = type != LightType::DIRECTIONAL;
	}

	bool dirty = true;
	bool mShadowMatricesDirty = true;

	bool validate();
	void recomputeLightProjection() const;	// Only used for shadow mapping

	void sendLightDataToShader(Shader& shader);
	void sendLightFarPlaneToShader(Shader& shader);
	void sendLightLightPosToShader(Shader& shader);
	void sendShadowMatrices(Shader& shader);

	void setlightMesh(std::unique_ptr<MeshComponent> lM);
	MeshComponent* getMesh();
	const glm::mat4& getLightProjectionMatrix() const;
	const std::array<glm::mat4, 6>& getLightViewMatrices();
	inline float getAspectRatio() const;

	std::unique_ptr<MeshComponent> lightMesh;
	std::unique_ptr<ShadowMap> mShadowMap;

private:

	void updateShadowMatrices();

	// Hard code for now
	float FOV = 90.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Shadow Matrices
	std::array<glm::mat4, 6> mLightViews;
	mutable glm::mat4 mLightProjection;
	mutable bool mlightProjectionDirty = true;
	bool isPerspective = true; // Perspective needed for Spot and Point, Ortho for directional

	float directionalShadowDistance = 5.0f;	// For View matrix calculation
	float directionalShadowExtent = 1.0f;	// For Orthographic Projection calculation
};