#pragma once

#include "Scene.h"
#include "EngineConfig.h"

class SceneLoader
{
public:
	SceneLoader(Project::ProjectNumber project);
	~SceneLoader() = default;

	std::unique_ptr<Scene> createBasicScene();

private:

	std::unique_ptr<Light> createLight(LightType type, Window* win);
	void createProject(Project::ProjectNumber proj, Scene* scene);
	Project::ProjectNumber project;
};
