#include "Renderer.h"

Renderer::Renderer(Window& win, ShaderManager& sm) : window(win), shaderManager(sm)
{
}

Renderer::~Renderer()
{
}

void Renderer::BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderFrame(Scene* scene, InputManager* inputManager, float dt)
{
	BeginFrame();

	// Handle looking around and movement
	scene->getCamera().updateInputs(*inputManager, dt);
	scene->getCamera().updateViewProjection();
	
	// Avoid empty loop overhead
	if (scene->meshes.size() > 0) {
		for (auto mesh : scene->meshes) {

			Shader* curShader = shaderManager.get(mesh->getShaderName());

			if (!curShader) {
				LogRendererWarn("Shader not found for " + mesh->getShaderName());
				continue;
			}

			// Use appropriate shader's to draw object
			curShader->Activate();

			// Setup appropriate MVP matrix
			Camera& cam = scene->getCamera();

			// Setup the mvp matrix, for NOW MANUALLY DOING IT HERE, send model seperately to vertex shader
			glm::mat4 model = mesh->getModelMatrix() * cam.getDistanceScale();

			// Send model to Vertex Shader
			glUniformMatrix4fv(glGetUniformLocation(curShader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

			// Send view & projection matrix to Vertex Shader
			cam.sendToShader(*curShader);

			mesh->Draw();
		}
	}

	GL_CHECK_ERROR();
	EndFrame();
}

void Renderer::EndFrame()
{
	window.swapBuffers();
}