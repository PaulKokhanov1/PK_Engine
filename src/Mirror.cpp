#include "Mirror.h"

Mirror::Mirror(glm::vec3 pos) : m_worldPosition(pos)
{
	createMesh();
}

Mirror::~Mirror()
{
}

void Mirror::setPlaneMesh(std::unique_ptr<MeshComponent> pM)
{
	if (pM == nullptr) {
		std::cerr << "[Scene] : Current plane mesh being set is null" << std::endl;
		return;
	}
	this->m_planeMesh = std::move(pM);
}

MeshComponent& Mirror::getMirrorMesh()
{
	return *m_planeMesh;
}

glm::vec3 Mirror::getNormal() const
{
	return m_worldNormal;
}

glm::vec3 Mirror::getPoint() const
{
	return m_worldPosition;
}

void Mirror::createMesh()
{
	m_planeMesh = std::make_unique<MeshComponent>("OBJ/plane/plane.obj");

	// Transform plane mesh, ADD FUNCTIONALITY TO CHANGE TRANSFORM
	Transform tR;
	tR.translation = m_worldPosition;
	tR.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	tR.scale = glm::vec3(3.0f, 1.0f, 3.0f);
	m_planeMesh->setTransform(tR);
}
