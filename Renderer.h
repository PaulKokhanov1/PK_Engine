#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"LogRenderer.h"
#include "EngineConfig.h"
#include "Scene.h"
#include "Window.h"
#include"ShaderManager.h"
#include"GLDebug.h"
#include"FBO.h"
#include"Application.h"

class Renderer
{
public:
	Renderer(Window& win);
	~Renderer();

	void Clear();
	void CollectionPass(Scene* scene);
	void ReflectionPass(Scene* scene, float dt);
	void DrawPass(Scene* scene, float dt);
	void EnvMapPass(Scene* scene);
	void RenderToTexturePass(Scene* scene, float dt); // Used Explicity for Project 5
	void PostProcessPass(Scene* scene);
	void RenderFrame(Scene* scene, float dt);
	void RenderFrameRenderToTexture(Scene* scene, float dt);
	void RenderFrameWithReflections(Scene* scene, float dt); // Used Explicity for Project 6
	void DrawPlaneWithShader(Scene* scene);
	void EndFrame();

	void createFBO();
	void createReflectionFBO();
	void createEnvMapBackgroundObject();

private:
	bool hasRenderedToTexture = false;


	struct RenderItem {
		SubMesh* subMeshRef;
		MeshComponent* meshRef;
		glm::mat4 modelMatrix;

		RenderItem(MeshComponent* mID, SubMesh* sID, glm::mat4 mM) : meshRef(mID), subMeshRef(sID), modelMatrix(mM) {}
	};
	std::unordered_map<Shader*, std::vector<RenderItem>> shaderBucket;

	Window& window;
	std::unique_ptr<FBO> RenderToTextureFBO;
	std::unique_ptr<FBO> ReflectionFBO;
	MeshComponent* renderToTextureMesh;
	std::unique_ptr<MeshComponent> cubeMapMesh;

};


