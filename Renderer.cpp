#include "Renderer.h"

Renderer::Renderer(Window& win, ShaderManager& sm) : window(win), shaderManager(sm)
{
	createFBO();
}

Renderer::~Renderer()
{
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderFrame(Scene* scene, shared_ptr<InputManager> inputManager, float dt)
{

	// Handle all resizing
	if (window.needsResize) {
		auto [w, h] = window.getWindowDimensions();
		createFBO();
		scene->getCamera().setScreenDimensions(w, h);

		window.needsResize = false;
	}

	// Render to Texture Pass
	// --------------------------------------------------------------------------------------------------------------------------------------
	RenderToTextureFBO->Bind();
	Clear();

	// Eventual Workflow -> collect renderables from Scene -> group by material -> for each group bind shader and upload data -> for each mesh in group, upload transform and draw

	// Handle scene updates 
	scene->update(*inputManager, dt);

	// Setup appropriate MVP matrix
	Camera& cam = scene->getCamera();
	
	// Avoid empty loop overhead
	if (scene->meshes.size() > 0) {
		for (auto mesh : scene->meshes) {

			// compute Model Matrix per mesh
			glm::mat4 model = mesh->computeModelMatrix();

			// Upload all necessary submesh data, and currently upload all camera materices again (pretty redundant atm)
			for (auto subMesh : mesh->getSubMeshes()) {

				Shader* curShader = shaderManager.get(subMesh.material.getShaderName());

				if (!curShader) {
					LogRendererWarn("Shader not found for " + subMesh.material.getShaderName());
					continue;
				}

				// Use appropriate shader's to draw object
				curShader->Activate();

				// Send view & projection matrix to Vertex Shader
				cam.sendViewAndProjToShader(*curShader);
				cam.sendCamDistanceScaleToShader(*curShader);

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

				// Send model to Vertex Shader
				glUniformMatrix4fv(curShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

				mesh->DrawSubMesh(subMesh);


			}

		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------


	// Post-Process Pass
	// --------------------------------------------------------------------------------------------------------------------------------------

	if (!RenderToTextureFBO || !RenderToTextureFBO->getRenderedTexture()) {
		LogRendererWarn("FBO not prepared for Post-Process Pass");
	}

	// Generate Mip maps for the rendered texture
	RenderToTextureFBO->getRenderedTexture()->Bind();
	glGenerateMipmap(GL_TEXTURE_2D);
	Shader* curShader = shaderManager.get("framebuffer");

	if (!curShader) {
		LogRendererWarn("Shader not found for framebuffer");
	}

	RenderToTextureFBO->Unbind();
	Clear();

	RenderToTextureFBO->getRenderedTexture()->sendUniformToShader(*curShader, "screenTexture", 0);

	// Send view & projection matrix to Vertex Shader
	cam.sendViewAndProjToShader(*curShader);

	// Send model to Vertex Shader
	glm::mat4 model = scene->quadController->getMesh()->computeModelMatrix();
	glUniformMatrix4fv(curShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

	for (auto subMesh : scene->quadController->getMesh()->getSubMeshes()) {
		scene->quadController->getMesh()->DrawSubMesh(subMesh);
	}
	// --------------------------------------------------------------------------------------------------------------------------------------

	GL_CHECK_ERROR();
	EndFrame();
}

void Renderer::EndFrame()
{
	window.swapBuffers();
}

void Renderer::createFBO()
{
	auto [w, h] = window.getWindowDimensions();
	RenderToTextureFBO = make_unique<FBO>(h, w, h, w);
	RenderToTextureFBO->Construct();
}
