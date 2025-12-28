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

	} else {
		// Handle looking around and movement, only if user is not holding down CTRL
		camera->updateInputs(input, deltaTime);
	}

	camera->updateViewProjection();

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


