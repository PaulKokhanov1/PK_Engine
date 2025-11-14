#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include<vector>

#include "MeshComponent.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	~Scene();

	// Collections of pointers to meshes used in this scene
	std::vector<MeshComponent*> meshes;
	
	// Camera Instance, scene will manage memory of the camera
	std::unique_ptr<Camera> camera;

	void AddMesh(MeshComponent* mesh);

	// Getters and Setters

	void setCamera(std::unique_ptr<Camera> camera);
	Camera& getCamera();

private:

};


#endif // !SCENE_CLASS_H
