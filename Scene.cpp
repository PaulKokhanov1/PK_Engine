#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	// Will clear memory of meshes as per RAII
	meshes.clear();
}

void Scene::AddMesh(std::unique_ptr<MeshComponent> mesh)
{
	// Prevent adding empty mesh
	if (!mesh) {
		std::cerr << "[Scene] : Mesh is empty" << std::endl;
		return;
	}

	meshes.push_back(move(mesh));
}

void Scene::Addlight(std::unique_ptr<Light> light)
{
	// Prevent adding empty light
	if (!light) {
		std::cerr << "[Scene] : Light is empty" << std::endl;
		return;
	}

	lights.push_back(move(light));
}

void Scene::update(float deltaTime)
{
	InputManager* input = Application::Get().getInputManager();

	if (input->isKeyHeld(GLFW_KEY_LEFT_CONTROL)) {
		
		if (lights.size()) lightController->update(deltaTime, lights);

	} else if (input->isKeyHeld(GLFW_KEY_LEFT_ALT)) {
		// Handle moving quad on which scene is rendered to
		if (quadController) quadController->update(deltaTime);

	} else {
		// Handle looking around and movement, only if user is not holding down CTRL
		if (camera) camera->updateInputs(deltaTime);
	}

	if (camera) camera->updateViewProjection();
	if (camera && m_mirror) camera->calcMirroredViewMatrix(m_mirror->getNormal(), m_mirror->getPoint());

}

void Scene::validate()
{
	if (camera == nullptr) {
		throw (SceneException("Current camera is not set!"));
	}
}

void Scene::createMirrorObject(glm::vec3 pos)
{
	m_mirror = std::make_unique<Mirror>(pos);
}

void Scene::setCamera(std::unique_ptr<Camera> camera)
{
	if (camera == nullptr) {
		std::cerr << "[Scene] : Current Camera being set it null" << std::endl;
		return;
	}

	this->camera = std::move(camera);
}

void Scene::setQuadController(std::unique_ptr<QuadController> qC)
{
	if (qC == nullptr) {
		std::cerr << "[Scene] : Current quad Mesh being set is null" << std::endl;
		return;
	}
	this->quadController = std::move(qC);
}

void Scene::setLightController(std::unique_ptr<LightController> lC)
{
	if (lC == nullptr) {
		std::cerr << "[Scene] : Light Controller being set is null" << std::endl;
		return;
	}
	this->lightController = std::move(lC);
}

void Scene::setCubeMap(std::unique_ptr<CubeMap> cM)
{
	if (cM == nullptr) {
		std::cerr << "[Scene] : Current cube map being set is null" << std::endl;
		return;
	}
	this->cubeMap = std::move(cM);
}

Camera& Scene::getCamera()
{
	// As we hold a unique ptr to camera, when scene is destroyed any objects holding a reference to this camera will no longer be able to access it
	// Though this may cause undefined behavior if this camera doesn't outlive the objects calling getCamera()
	return *camera;
}

QuadController& Scene::getQuadController()
{
	return *quadController;
}

LightController& Scene::getLightController()
{
	return *lightController;
}

CubeMap& Scene::getCubeMap()
{
	return *cubeMap;
}

Mirror& Scene::getMirror()
{
	return *m_mirror;
}

std::vector<std::unique_ptr<Light>>& Scene::getLights()
{
	return lights;
}

std::vector<std::unique_ptr<MeshComponent>>& Scene::getMeshes()
{
	return meshes;
}


