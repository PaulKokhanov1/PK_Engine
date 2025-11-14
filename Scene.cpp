#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	// Do Not delete meshes as Scene component does not own them
	meshes.clear();
}

void Scene::AddMesh(MeshComponent* mesh)
{
	// Prevent adding empty mesh
	if (!mesh) {
		std::cerr << "[Scene] : Mesh is empty" << std::endl;
		return;
	}

	meshes.push_back(mesh);
}

void Scene::setCamera(std::unique_ptr<Camera> camera)
{
	if (camera == nullptr) {
		std::cerr << "[Scene] : Current Camera being set it null" << std::endl;
		return;
	}

	this->camera = std::move(camera);
}

Camera& Scene::getCamera()
{
	// As we hold a unique ptr to camera, when scene is destroyed any objects holding a reference to this camera will no longer be able to access it
	return *camera;
}


