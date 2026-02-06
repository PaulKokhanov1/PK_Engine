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
#include"FBO.h"

class Renderer
{
public:
	Renderer(Window& win, ShaderManager& sm);
	~Renderer();

	void Clear();
	void RenderFrame(Scene* scene, shared_ptr<InputManager> inputManager, float dt);
	void EndFrame();

	void createFBO();

private:

	Window& window;
	ShaderManager& shaderManager;
	unique_ptr<FBO> RenderToTextureFBO;
	MeshComponent* renderToTextureMesh;
};


#endif // !RENDERER_CLASS_H


