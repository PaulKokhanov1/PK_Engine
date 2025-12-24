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

	// Handle scene updates 
	scene->update(*inputManager, dt);
	
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

			// Upload material data to Vertex Shader
			auto [Ka, Kd, Ks, shininess] = mesh->getMaterial().getAttributes();
			glUniform3fv(glGetUniformLocation(curShader->ID, "Ka"), 1, glm::value_ptr(Ka));
			glUniform3fv(glGetUniformLocation(curShader->ID, "Kd"), 1, glm::value_ptr(Kd));
			glUniform3fv(glGetUniformLocation(curShader->ID, "Ks"), 1, glm::value_ptr(Ks));
			glUniform1f(glGetUniformLocation(curShader->ID, "shininess"), shininess);


			// Upload light data to Vertex Shader, may be inefficient to loop through all lights per mesh, potentially group meshes, lights and other obj's per shader
			for (auto light : scene->lights) {
				glUniform3fv(glGetUniformLocation(curShader->ID, "lightPosWorld"), 1, glm::value_ptr(light->position));
				glUniform3fv(glGetUniformLocation(curShader->ID, "lightColor"), 1, glm::value_ptr(light->color));
				glUniform3fv(glGetUniformLocation(curShader->ID, "lightKa"), 1, glm::value_ptr(light->ambient));
				glUniform3fv(glGetUniformLocation(curShader->ID, "lightKd"), 1, glm::value_ptr(light->diffuse));
				glUniform3fv(glGetUniformLocation(curShader->ID, "lightKs"), 1, glm::value_ptr(light->specular));
			}

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