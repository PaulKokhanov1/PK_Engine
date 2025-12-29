#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"LogRenderer.h"
#include "Scene.h"
#include "Window.h"
#include"ShaderManager.h"
#include"GLDebug.h"

class Renderer
{
public:
	Renderer(Window& win, ShaderManager& sm);
	~Renderer();

	void BeginFrame();
	void RenderFrame(Scene* scene, InputManager* inputManagers, float dt);
	void EndFrame();

private:

	Window& window;
	ShaderManager& shaderManager;
};


#endif // !RENDERER_CLASS_H


