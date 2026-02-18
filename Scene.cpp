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

	meshes.insert(mesh);
}

void Scene::Addlight(Light* light)
{
	// Prevent adding empty light
	if (!light) {
		std::cerr << "[Scene] : Light is empty" << std::endl;
		return;
	}

	lights.push_back(light);
}

void Scene::update(InputManager& input, float deltaTime)
{
	if (input.isKeyHeld(GLFW_KEY_LEFT_CONTROL)) {
		
		// Adjust position of light around origin
		if (input.isMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {

			// Currently we'll adjust each light in this way
			for (auto& light : lights) {

				light->yaw += lightMovementSensitivity * input.getDeltaMouseX();
				light->pitch += lightMovementSensitivity * input.getDeltaMouseY();

				float dirX = cos(glm::radians(light->yaw)) * cos(glm::radians(light->pitch));
				float dirY = -sin(glm::radians(light->pitch));
				float dirZ = sin(glm::radians(light->yaw)) * cos(glm::radians(light->pitch));


				light->position.x = light->radius * dirX;
				//light->position.y = light->radius * dirY;
				light->position.z = light->radius * dirZ;


			}

		}

	} else if (input.isKeyHeld(GLFW_KEY_LEFT_ALT)) {
		// Handle moving quad on which scene is rendered to
		if (quadController) quadController->update(input, deltaTime);

	} else {
		// Handle looking around and movement, only if user is not holding down CTRL
		if (camera) camera->updateInputs(input, deltaTime);
	}

	if (camera) camera->updateViewProjection();

}

void Scene::validate()
{
	if (camera == nullptr) {
		throw (SceneException("Current camera is not set!"));
	}
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
		std::cerr << "[Scene] : Current quad Mesh being set it null" << std::endl;
		return;
	}
	this->quadController = std::move(qC);
}

void Scene::setCubeMap(std::unique_ptr<CubeMap> cM)
{
	if (cM == nullptr) {
		std::cerr << "[Scene] : Current cube map being set it null" << std::endl;
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

CubeMap& Scene::getCubeMap()
{
	return *cubeMap;
}

std::vector<Light*> Scene::getLights()
{
	return lights;
}

std::set<MeshComponent*> Scene::getMeshes()
{
	return meshes;
}


