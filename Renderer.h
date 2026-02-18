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
	void CollectionPass(Scene* scene);
	void DrawPass(Scene* scene, shared_ptr<InputManager> inputManager, float dt);
	void EnvMapPass(Scene* scene);
	void PostProcessPass(Scene* scene);
	void RenderFrame(Scene* scene, shared_ptr<InputManager> inputManager, float dt);
	void RenderFrameRenderToTexture(Scene* scene, shared_ptr<InputManager> inputManager, float dt);
	void EndFrame();

	void createFBO();
	void createEnvMapBackgroundObject();

private:

	struct RenderItem {
		SubMesh* subMeshRef;
		MeshComponent* meshRef;
		glm::mat4 modelMatrix;

		RenderItem(MeshComponent* mID, SubMesh* sID, glm::mat4 mM) : meshRef(mID), subMeshRef(sID), modelMatrix(mM) {}
	};
	unordered_map<Shader*, vector<RenderItem>> shaderBucket;

	Window& window;
	ShaderManager& shaderManager;
	unique_ptr<FBO> RenderToTextureFBO;
	MeshComponent* renderToTextureMesh;
	MeshComponent* cubeMapMesh;

};


#endif // !RENDERER_CLASS_H


