#pragma once

#include "MeshComponent.h"

class Mirror
{
public:
	Mirror(glm::vec3 pos);
	~Mirror();

	void setPlaneMesh(std::unique_ptr<MeshComponent> pM);

	MeshComponent& getMirrorMesh();
	glm::vec3 getNormal() const;
	glm::vec3 getPoint() const;


private:

	void createMesh();

	// Single Plane for rendering reflections on it
	std::unique_ptr<MeshComponent> m_planeMesh;

	glm::vec3 m_worldPosition;
	glm::vec3 m_worldNormal = glm::vec3(0.0f, 1.0f, 0.0f);

};
