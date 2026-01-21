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

	// Eventual Workflow -> collect renderables from Scene -> group by material -> for each group bind shader and upload data -> for each mesh in group, upload transform and draw

	// Handle scene updates 
	scene->update(*inputManager, dt);
	
	// Avoid empty loop overhead
	if (scene->meshes.size() > 0) {
		for (auto mesh : scene->meshes) {

			// Upload all necessary submesh data, and currently upload all camera materices again (pretty redundant atm)
			for (auto subMesh : mesh->getSubMeshes()) {

				Shader* curShader = shaderManager.get(subMesh.material.getShaderName());

				if (!curShader) {
					LogRendererWarn("Shader not found for " + subMesh.material.getShaderName());
					continue;
				}

				// Use appropriate shader's to draw object
				curShader->Activate();

				// Setup appropriate MVP matrix
				Camera& cam = scene->getCamera();


				// Send model to Vertex Shader
				glm::mat4 model = mesh->getModelMatrix() * cam.getDistanceScale();
				glUniformMatrix4fv(curShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

				// Send view & projection matrix to Vertex Shader
				cam.sendToShader(*curShader);

				// Upload material data to Vertex Shader
				subMesh.material.uploadData(*curShader);

				// Upload light data to Vertex Shader, may be inefficient to loop through all lights per mesh, potentially group meshes, lights and other obj's per shader
				for (auto light : scene->lights) {
					if (light->dirty) light->validate();
					glUniform3fv(curShader->getUniformLocation("lightPosWorld"), 1, glm::value_ptr(light->position));
					glUniform3fv(curShader->getUniformLocation("lightColor"), 1, glm::value_ptr(light->color));
					glUniform3fv(curShader->getUniformLocation("lightKa"), 1, glm::value_ptr(light->ambient));
					glUniform3fv(curShader->getUniformLocation("lightKd"), 1, glm::value_ptr(light->diffuse));
					glUniform3fv(curShader->getUniformLocation("lightKs"), 1, glm::value_ptr(light->specular));
				}

				mesh->DrawSubMesh(subMesh);


			}

		}
	}

	GL_CHECK_ERROR();
	EndFrame();
}

void Renderer::EndFrame()
{
	window.swapBuffers();
}
