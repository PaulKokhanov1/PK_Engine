#include "Renderer.h"

Renderer::Renderer(Window& win) : window(win)
{
	createFBO();
	createReflectionFBO();
	createEnvMapBackgroundObject();
}

Renderer::~Renderer()
{
}

void Renderer::Clear()
{
	// Background color, clear alpha channel so that rendered texture with object its placed on allows for mixing colors
	glClearColor(0.0f, 0.f, 0.f, 0.0f);

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
			Shader* shader = Application::Get().getShaderManager()->get(subMesh.material.getShaderName());
			if (!shader) continue;

			shaderBucket[shader].emplace_back(mesh.get(), &subMesh, model);
		}
	}

	// handle light objects aswell
	for (auto& light : scene->getLights()) {
		if (!light->lightMesh) continue;
		if (!light->shouldShowMesh) continue;

		// compute Model Matrix per mesh
		glm::mat4 model = light->lightMesh->computeModelMatrix();

		for (auto& subMesh : light->lightMesh->getSubMeshes()) {
			// Create RenderItems and store by shader
			Shader* shader = Application::Get().getShaderManager()->get(subMesh.material.getShaderName());
			if (!shader) continue;

			shaderBucket[shader].emplace_back(light->lightMesh.get(), &subMesh, model);
		}
	}
}

void Renderer::ReflectionPass(Scene* scene, float dt)
{
	// Reflection Render to Texture Pass, Only works for reflecting ONE object
	ReflectionFBO->Bind();
	Clear();

	Camera& cam = scene->getCamera();

	for (auto& [shader, renderItems] : shaderBucket) {

		if (!shader) {
			LogRendererWarn("Shader not found for " + shader->getVertexFile());
			continue;
		}

		// Use appropriate shader's to draw object
		shader->Activate();

		// Send mirrored view & projection matrix to Vertex Shader
		cam.sendMirroredViewAndProjToShader(*shader);
		cam.sendCamPositionWorldSpaceToShader(*shader);

		// Upload cubeMap
		scene->getCubeMap().sendUniformToShader(*shader, "env");

		// Upload light data to Vertex Shader, once per shader
		for (auto& light : scene->getLights()) {
			light->sendLightDataToShader(*shader);
		}

		// Iterate through all assciated subMeshes
		for (auto& renderItem : renderItems) {

			// Upload material data to Vertex Shader
			renderItem.subMeshRef->material.uploadData(*shader);

			// Send model to Vertex Shader
			shader->setUniformMat4fv("modelMatrix", renderItem.modelMatrix);

			// Draw sub Mesh
			renderItem.meshRef->DrawSubMesh(*renderItem.subMeshRef);
		}

	}

	ReflectionFBO->Unbind();
}

void Renderer::DrawPass(Scene* scene, float dt)
{

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
		cam.sendCamPositionWorldSpaceToShader(*shader);

		// Upload cubeMap
		scene->getCubeMap().sendUniformToShader(*shader, "env");

		// Upload light data to Vertex Shader, once per shader
		for (auto& light : scene->getLights()) {
			light->sendLightDataToShader(*shader);
		}


		// Iterate through all assciated subMeshes
		for (auto& renderItem : renderItems) {

			// Upload material data to Vertex Shader
			renderItem.subMeshRef->material.uploadData(*shader);

			// Send model to Vertex Shader
			shader->setUniformMat4fv("modelMatrix", renderItem.modelMatrix);

			// Send normalMatrix used for env reflections and lights
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(renderItem.modelMatrix));
			shader->setUniformMat3fv("normalMatrix", normalMatrix);

			// Draw sub Mesh
			renderItem.meshRef->DrawSubMesh(*renderItem.subMeshRef);
		}

	}
}

void Renderer::EnvMapPass(Scene* scene)
{
	// Upload Uniforms

	Shader* envShader = Application::Get().getShaderManager()->get("skybox");

	if (!envShader) {
		LogRendererWarn("Shader not found for drawing environment");
	}

	// Setup appropriate MVP matrix
	Camera& cam = scene->getCamera();

	// Use appropriate shader's to draw object
	envShader->Activate();

	// uniforms
	cam.sendInverseProjViewToShader(*envShader);
	cam.sendCamPositionWorldSpaceToShader(*envShader);

	// Upload cubeMap
	scene->getCubeMap().sendUniformToShader(*envShader, "env");

	// Send clip space coords of Env Map Triangle to Vertex Shader
	for (auto subMesh : cubeMapMesh->getSubMeshes()) {
		cubeMapMesh->DrawSubMesh(subMesh);
	}
}

void Renderer::RenderToTexturePass(Scene* scene, float dt)
{
	// Render to Texture Pass
	RenderToTextureFBO->Bind();
	Clear();

	DrawPass(scene, dt);

	RenderToTextureFBO->Unbind();
}

void Renderer::PostProcessPass(Scene* scene)
{

	if (!RenderToTextureFBO || !RenderToTextureFBO->getRenderedTexture()) {
		LogRendererWarn("FBO not prepared for Post-Process Pass");
	}

	// Generate Mip maps for the rendered texture
	RenderToTextureFBO->getRenderedTexture()->Bind();
	glGenerateMipmap(GL_TEXTURE_2D);
	Shader* curShader = Application::Get().getShaderManager()->get("framebuffer");

	if (!curShader) {
		LogRendererWarn("Shader not found for framebuffer");
	}

	RenderToTextureFBO->sendUniformToShader(*curShader, "screenTexture");

	// Send view & projection matrix to Vertex Shader
	scene->getCamera().sendViewAndProjToShader(*curShader);

	// Send model to Vertex Shader
	glm::mat4 model = scene->getQuadController().getMesh()->computeModelMatrix();
	curShader->setUniformMat4fv("modelMatrix", model);

	for (auto subMesh : scene->getQuadController().getMesh()->getSubMeshes()) {
		scene->getQuadController().getMesh()->DrawSubMesh(subMesh);
	}
}

void Renderer::RenderFrame(Scene* scene, float dt)
{

	// Handle all resizing
	if (window.needsResize) {
		auto [w, h] = window.getWindowDimensions();
		createFBO();
		scene->getCamera().setScreenDimensions(w, h);

		window.needsResize = false;
	}

	// Handle scene updates 
	scene->update(dt);

	Clear();
	CollectionPass(scene);

	DrawPass(scene, dt);

	// Removing overdraw when drawing background
	glDepthMask(GL_FALSE);
	EnvMapPass(scene);
	glDepthMask(GL_TRUE);

	GL_CHECK_ERROR();
	EndFrame();
}

void Renderer::RenderFrameRenderToTexture(Scene* scene, float dt)
{
	
	// Handle all resizing
	if (window.needsResize) {
		auto [w, h] = window.getWindowDimensions();
		createFBO();
		scene->getCamera().setScreenDimensions(w, h);

		window.needsResize = false;
	}

	// Handle scene updates 
	scene->update(dt);

	CollectionPass(scene);

	// Runs ONCE atm, this is used in order to keep the teapot mapped to the quad's rotation and position,
	// if you want to use projectiveTextureMapping then just remove the "runs once atm" code and just run RenderToTexturePass
	if (!hasRenderedToTexture) {
		RenderToTexturePass(scene, dt);
		hasRenderedToTexture = true;
	}

	Clear(); // Needed here as RenderToTexturePass runs ONCE atm

	// Post-Process Pass
	PostProcessPass(scene);

	// Removing overdraw when drawing background
	glDepthMask(GL_FALSE);
	EnvMapPass(scene);
	glDepthMask(GL_TRUE);

	GL_CHECK_ERROR();
	EndFrame();
}

void Renderer::RenderFrameWithReflections(Scene* scene, float dt)
{

	// Handle all resizing
	if (window.needsResize) {
		auto [w, h] = window.getWindowDimensions();
		createFBO();
		scene->getCamera().setScreenDimensions(w, h);

		window.needsResize = false;
	}

	// Handle scene updates 
	scene->update(dt);

	Clear();
	CollectionPass(scene);
	ReflectionPass(scene, dt);

	DrawPass(scene, dt);

	// Draw Mirror Plane to with its own shader
	DrawPlaneWithShader(scene);

	// Removing overdraw when drawing background
	glDepthMask(GL_FALSE);
	EnvMapPass(scene);
	glDepthMask(GL_TRUE);

	GL_CHECK_ERROR();
	EndFrame();
}

void Renderer::DrawPlaneWithShader(Scene* scene)
{
	if (!ReflectionFBO || !ReflectionFBO->getRenderedTexture()) {
		LogRendererWarn("FBO not prepared for Draw Plane with Shader Pass");
	}

	// Generate Mip maps for the rendered texture
	ReflectionFBO->getRenderedTexture()->Bind();
	glGenerateMipmap(GL_TEXTURE_2D);

	Shader* planeReflectionShader = Application::Get().getShaderManager()->get("planeReflection");

	if (!planeReflectionShader) {
		LogRendererWarn("Shader not found for plane reflections");
	}

	ReflectionFBO->sendUniformToShader(*planeReflectionShader, "screenTexture");

	// Upload cubeMap
	scene->getCubeMap().sendUniformToShader(*planeReflectionShader, "env");

	// Send needed uniforms to shader
	scene->getCamera().sendViewAndProjToShader(*planeReflectionShader);
	scene->getCamera().sendMirroredViewToShader(*planeReflectionShader);
	scene->getCamera().sendCamPositionWorldSpaceToShader(*planeReflectionShader);

	// Upload env light intensity and specular lighting to Vertex Shader
	for (auto& light : scene->getLights()) {
		if (light->dirty) light->validate();
		planeReflectionShader->setUniform1f("envLightIntensity", light->envLightIntensity);
		planeReflectionShader->setUniform3fv("lightKs", light->specular);
	}

	// Send model and its inverse to Vertex Shader
	glm::mat4 model = scene->getMirror().getMirrorMesh().computeModelMatrix();
	planeReflectionShader->setUniformMat4fv("modelMatrix", model);
	planeReflectionShader->setUniformMat3fv("normalMatrix", glm::transpose(glm::inverse(model)));

	for (auto subMesh : scene->getMirror().getMirrorMesh().getSubMeshes()) {
		scene->getMirror().getMirrorMesh().DrawSubMesh(subMesh);
	}
}

void Renderer::EndFrame()
{
	window.swapBuffers();
}

void Renderer::createFBO()
{
	auto [w, h] = window.getWindowDimensions();
	RenderToTextureFBO = std::make_unique<FBO>(4096, 4096, w, h); // use higher resolution texture width and height to allow for better magnification
	RenderToTextureFBO->Construct();
}

void Renderer::createReflectionFBO()
{
	auto [w, h] = window.getWindowDimensions();
	ReflectionFBO = std::make_unique<FBO>(engineConfig::REFLECTION_RESOLUTION_WIDTH, engineConfig::REFLECTION_RESOLUTION_HEIGHT, w, h);
	ReflectionFBO->Construct();
}

void Renderer::createEnvMapBackgroundObject()
{
	cubeMapMesh = std::make_unique<MeshComponent>("OBJ/Triangle/triangle.obj", false);
	cubeMapMesh->getSubMeshes();
	for (auto& submesh : cubeMapMesh->getSubMeshes()) {
		submesh.material.setShaderName("skybox");
	}

}
