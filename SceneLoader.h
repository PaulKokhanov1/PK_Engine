#pragma once

#include "Scene.h"

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

	std::unique_ptr<Scene> createBasicScene();

private:

	std::unique_ptr<Light> createLight(LightType type, Window* win);
};
