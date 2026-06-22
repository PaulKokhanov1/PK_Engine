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
#include"ShadowMap.h"

// Using Bitflags to easier check if object should be rendered
enum RenderLayer {
	MAIN_PASS = 1,
	SHADOW_CASTER = 2,
	DEBUG = 4
};

class Renderer
{
public:
	Renderer(Window& win);
	~Renderer();

	void Clear();
	void CollectionPass(Scene* scene);
	void ReflectionPass(Scene* scene, float dt);
	void ShadowPass(Scene* scene, float dt);	// Generates texture (just depth) of scene seen from light
	void RenderDepthTextureShadow(Scene* scene, float dt, Light* light, Shader* shader);
	void RenderDepthCubemapShadow(Scene* scene, float dt, Light* light, Shader* shader);
	void DrawPass(Scene* scene, float dt);
	void EnvMapPass(Scene* scene);
	void RenderToTexturePass(Scene* scene, float dt); // Used Explicity for Project 5
	void PostProcessPass(Scene* scene);
	void TempShadowMapRenderToQuadPass(Scene* scene);
	void RenderFrame(Scene* scene, float dt);
	void RenderFrameRenderToTexture(Scene* scene, float dt);
	void RenderFrameWithReflections(Scene* scene, float dt); // Used Explicity for Project 6
	void DrawPlaneWithShader(Scene* scene);
	void EndFrame();

	void createFBO();	// Move GBO creation elsewhere, i.e FBOFactory or something
	void createReflectionFBO();
	void createEnvMapBackgroundObject();

	// Upload code (to uniforms)
	void UploadEnvironmentData(Shader* shader, Scene* scene);
	void UploadCameraData(Shader* shader, Scene* scene);

private:
	bool hasRenderedToTexture = false;


	struct RenderItem {
		SubMesh* subMeshRef;
		MeshComponent* meshRef;
		glm::mat4 modelMatrix;
		uint32_t renderLayer;

		RenderItem(MeshComponent* mID, SubMesh* sID, glm::mat4 mM, uint32_t rL) : meshRef(mID), subMeshRef(sID), modelMatrix(mM), renderLayer(rL) {}
	};
	std::unordered_map<Shader*, std::vector<RenderItem>> shaderBucket;

	Window& window;
	ShaderManager* shaderManager; // Could also call Application, but just cleaner this way
	std::unique_ptr<FBO> RenderToTextureFBO;
	std::unique_ptr<FBO> ReflectionFBO;
	MeshComponent* renderToTextureMesh;
	std::unique_ptr<MeshComponent> cubeMapMesh;

};


