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
				glUniformMatrix4fv(glGetUniformLocation(curShader->ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

				// Send view & projection matrix to Vertex Shader
				cam.sendToShader(*curShader);

				// Upload material data to Vertex Shader
				auto [Ka, Kd, Ks, shininess] = subMesh.material.getAttributes();
				glUniform3fv(glGetUniformLocation(curShader->ID, "Ka"), 1, glm::value_ptr(Ka));
				glUniform3fv(glGetUniformLocation(curShader->ID, "Kd"), 1, glm::value_ptr(Kd));
				glUniform3fv(glGetUniformLocation(curShader->ID, "Ks"), 1, glm::value_ptr(Ks));
				glUniform1f(glGetUniformLocation(curShader->ID, "shininess"), shininess);

				// Upload textures to Vertex Shader, assuming one smapler type per shader
				auto [difTexture, ambTexture, specTexture] = subMesh.material.getTextures();

				if (difTexture) {
					difTexture->sendUniformToShader(*curShader, "texDiff");
					difTexture->Bind();
				}
				if (ambTexture) {
					ambTexture->sendUniformToShader(*curShader, "texAmb");
					ambTexture->Bind();

				}
				if (specTexture) {
					specTexture->sendUniformToShader(*curShader, "texSpec");
					specTexture->Bind();
				}


				// Upload light data to Vertex Shader, may be inefficient to loop through all lights per mesh, potentially group meshes, lights and other obj's per shader
				for (auto light : scene->lights) {
					if (light->dirty) light->validate();
					glUniform3fv(glGetUniformLocation(curShader->ID, "lightPosWorld"), 1, glm::value_ptr(light->position));
					glUniform3fv(glGetUniformLocation(curShader->ID, "lightColor"), 1, glm::value_ptr(light->color));
					glUniform3fv(glGetUniformLocation(curShader->ID, "lightKa"), 1, glm::value_ptr(light->ambient));
					glUniform3fv(glGetUniformLocation(curShader->ID, "lightKd"), 1, glm::value_ptr(light->diffuse));
					glUniform3fv(glGetUniformLocation(curShader->ID, "lightKs"), 1, glm::value_ptr(light->specular));
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
