#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include<vector>
#include<set>
#include<glm/glm.hpp>

#include "MeshComponent.h"
#include "Camera.h"
#include "Light.h"
#include "SceneException.h"
#include "QuadController.h"
#include "CubeMap.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AddMesh(MeshComponent* mesh);
	void Addlight(Light* light);

	// Constant Scene updates
	void update(InputManager& input, float deltaTime);

	// Used prior to rendering to ensure all needed objects are created for a scene to be created
	void validate();

	// Getters and Setters

	void setCamera(std::unique_ptr<Camera> camera);
	void setQuadController(std::unique_ptr<QuadController> qC);
	void setCubeMap(std::unique_ptr<CubeMap> cM);
	Camera& getCamera();
	QuadController& getQuadController();
	CubeMap& getCubeMap();
	std::vector<Light*> getLights();
	std::set<MeshComponent*> getMeshes();

private:

	// Light movement sensitivity
	float lightMovementSensitivity = 1.0f;

	// Collections of pointers to meshes used in this scene
	std::set<MeshComponent*> meshes;

	// Used for controlling position ofva pre-processed texture quad
	std::unique_ptr<QuadController> quadController;

	// Collections of pointers to lights in used in this scene
	std::vector<Light*> lights;

	// Camera Instance, scene will manage memory of the camera
	std::unique_ptr<Camera> camera;

	// Single Cube Map, at the moment no need for more
	std::unique_ptr<CubeMap> cubeMap;

};


#endif // !SCENE_CLASS_H
