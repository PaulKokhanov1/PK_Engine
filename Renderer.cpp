#include "Renderer.h"

Renderer::Renderer(Window& win, ShaderManager& sm) : window(win), shaderManager(sm)
{
	createFBO();
	createEnvMapBackgroundObject();
}

Renderer::~Renderer()
{
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::CollectionPass(Scene* scene)
{
	shaderBucket.clear();
	for (auto& mesh : scene->getMeshes()) {

		// compute Model Matrix per mesh
		glm::mat4 model = mesh->computeModelMatrix();

		for (auto& subMesh : mesh->getSubMeshes()) {
			// Create RenderItems and store by shader
			Shader* shader = shaderManager.get(subMesh.material.getShaderName());
			if (!shader) continue;

			shaderBucket[shader].emplace_back(mesh, &subMesh, model);
		}
	}
}

void Renderer::DrawPass(Scene* scene, shared_ptr<InputManager> inputManager, float dt)
{

	// Handle scene updates 
	scene->update(*inputManager, dt);

	// Setup appropriate MVP matrix
	Camera& cam = scene->getCamera();

	for (auto& [shader, renderItems] : shaderBucket) {

		if (!shader) {
			LogRendererWarn("Shader not found for " + shader->getVertexFile());
			continue;
		}

		// Use appropriate shader's to draw object
		shader->Activate();

		// Send view & projection matrix to Vertex Shader
		cam.sendViewAndProjToShader(*shader);
		cam.sendCamDistanceScaleToShader(*shader);
		cam.sendCamPositionWorldSpaceToShader(*shader);

		// Upload cubeMap
		scene->getCubeMap().sendUniformToShader(*shader, "env", 0);

		// Upload light data to Vertex Shader, once per shader
		for (auto light : scene->getLights()) {
			if (light->dirty) light->validate();
			glUniform3fv(shader->getUniformLocation("lightPosWorld"), 1, glm::value_ptr(light->position));
			glUniform3fv(shader->getUniformLocation("lightColor"), 1, glm::value_ptr(light->color));
			glUniform3fv(shader->getUniformLocation("lightKa"), 1, glm::value_ptr(light->ambient));
			glUniform3fv(shader->getUniformLocation("lightKd"), 1, glm::value_ptr(light->diffuse));
			glUniform3fv(shader->getUniformLocation("lightKs"), 1, glm::value_ptr(light->specular));
		}


		// Iterate through all assciated subMeshes
		for (auto& renderItem : renderItems) {

			// Upload material data to Vertex Shader
			renderItem.subMeshRef->material.uploadData(*shader);

			// Send model to Vertex Shader
			glUniformMatrix4fv(shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(renderItem.modelMatrix));

			// Draw sub Mesh
			renderItem.meshRef->DrawSubMesh(*renderItem.subMeshRef);
		}

	}
}

void Renderer::EnvMapPass(Scene* scene)
{
	// Upload Uniforms

	Shader* envShader = shaderManager.get("skybox");

	if (!envShader) {
		LogRendererWarn("Shader not found for drawing environment");
	}

	// Setup appropriate MVP matrix
	Camera& cam = scene->getCamera();

	// Use appropriate shader's to draw object
	envShader->Activate();

	// Send view & projection matrix to Vertex Shader
	cam.sendViewAndProjToShader(*envShader);
	cam.sendCamPositionWorldSpaceToShader(*envShader);

	// Upload cubeMap
	scene->getCubeMap().sendUniformToShader(*envShader, "env", 0);

	// Send clip space coords of Env Map Triangle to Vertex Shader
	for (auto subMesh : cubeMapMesh->getSubMeshes()) {
		cubeMapMesh->DrawSubMesh(subMesh);
	}
}

void Renderer::PostProcessPass(Scene* scene)
{

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
	scene->getCamera().sendViewAndProjToShader(*curShader);

	// Send model to Vertex Shader
	glm::mat4 model = scene->getQuadController().getMesh()->computeModelMatrix();
	glUniformMatrix4fv(curShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

	for (auto subMesh : scene->getQuadController().getMesh()->getSubMeshes()) {
		scene->getQuadController().getMesh()->DrawSubMesh(subMesh);
	}
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

	Clear();

	CollectionPass(scene);
	DrawPass(scene, inputManager, dt);

	// Removing overdraw when drawing background
	glDepthMask(GL_FALSE);
	EnvMapPass(scene);
	glDepthMask(GL_TRUE);

	GL_CHECK_ERROR();
	EndFrame();
}

void Renderer::RenderFrameRenderToTexture(Scene* scene, shared_ptr<InputManager> inputManager, float dt)
{

	// Handle all resizing
	if (window.needsResize) {
		auto [w, h] = window.getWindowDimensions();
		createFBO();
		scene->getCamera().setScreenDimensions(w, h);

		window.needsResize = false;
	}

	// Render to Texture Pass
	RenderToTextureFBO->Bind();
	Clear();

	CollectionPass(scene);
	DrawPass(scene, inputManager, dt);

	// Post-Process Pass
	PostProcessPass(scene);

	// Removing overdraw when drawing background
	glDepthMask(GL_FALSE);
	EnvMapPass(scene);
	glDepthMask(GL_TRUE);

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

void Renderer::createEnvMapBackgroundObject()
{
	cubeMapMesh = new MeshComponent("OBJ/Triangle/triangle.obj", false);
	cubeMapMesh->getSubMeshes();
	for (auto& submesh : cubeMapMesh->getSubMeshes()) {
		submesh.material.setShaderName("skybox");
	}

	std::cout << "[DBG] Manual mesh created: vertices=" << cubeMapMesh->getVertices().size()
		<< " indices=" << cubeMapMesh->getIndices().size()
		<< " submeshes=" << cubeMapMesh->getSubMeshes().size() << std::endl;

}
