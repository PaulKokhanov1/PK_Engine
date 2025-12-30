#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include<vector>
#include<set>
#include<glm/glm.hpp>

#include "MeshComponent.h"
#include "Camera.h"
#include "Light.h"
#include "SceneException.h"

class Scene
{
public:
	Scene();
	~Scene();

	// Collections of pointers to meshes used in this scene
	std::set<MeshComponent*> meshes;

	// Collections of pointers to lights in used in this scene
	std::vector<Light*> lights;
	
	// Camera Instance, scene will manage memory of the camera
	std::unique_ptr<Camera> camera;

	void AddMesh(MeshComponent* mesh);
	void Addlight(Light* light);

	// Constant Scene updates
	void update(InputManager& input, float deltaTime);

	// Used prior to rendering to ensure all needed objects are created for a scene to be created
	void validate();

	// Getters and Setters

	void setCamera(std::unique_ptr<Camera> camera);
	Camera& getCamera();

private:

	// Light movement sensitivity
	float lightMovementSensitivity = 1.0f;

};


#endif // !SCENE_CLASS_H
