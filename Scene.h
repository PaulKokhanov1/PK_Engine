#pragma once

#include<vector>
#include<glm/glm.hpp>

#include "MeshComponent.h"
#include "Camera.h"
#include "Light.h"
#include "SceneException.h"
#include "QuadController.h"
#include "LightController.h"
#include "CubeMap.h"
#include "Application.h"
#include "Mirror.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AddMesh(std::unique_ptr<MeshComponent> mesh);
	void Addlight(std::unique_ptr<Light> light);

	// Constant Scene updates
	void update(float deltaTime);

	// Used prior to rendering to ensure all needed objects are created for a scene to be created
	void validate();

	void createMirrorObject(glm::vec3 pos);

	// Getters and Setters

	void setCamera(std::unique_ptr<Camera> camera);
	void setQuadController(std::unique_ptr<QuadController> qC);
	void setLightController(std::unique_ptr<LightController> qC);
	void setCubeMap(std::unique_ptr<CubeMap> cM);
	Camera& getCamera();
	QuadController& getQuadController();
	LightController& getLightController();
	CubeMap& getCubeMap();
	Mirror& getMirror();
	std::vector<std::unique_ptr<Light>>& getLights();
	std::vector<std::unique_ptr<MeshComponent>>& getMeshes();

private:

	// Collections of pointers to meshes used in this scene
	std::vector<std::unique_ptr<MeshComponent>> meshes;

	// Used for controlling position of pre-processed texture quad
	std::unique_ptr<QuadController> quadController;

	// Used for controlling position of lights in the scene
	std::unique_ptr<LightController> lightController;

	// Collections of pointers to lights in used in this scene
	std::vector<std::unique_ptr<Light>> lights;

	// Camera Instance, scene will manage memory of the camera
	std::unique_ptr<Camera> camera;

	// Single Cube Map, at the moment no need for more
	std::unique_ptr<CubeMap> cubeMap;

	// mirror for rendering reflections on it
	std::unique_ptr<Mirror> m_mirror;
};

