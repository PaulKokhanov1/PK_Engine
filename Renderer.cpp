#include "Renderer.h"

Renderer::Renderer(Window& win) : window(win)
{
	shaderManager = Application::Get().getShaderManager();

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
			Shader* shader = shaderManager->get(subMesh.material.getShaderName());
			if (!shader) continue;

			shaderBucket[shader].emplace_back(mesh.get(), &subMesh, model, (RenderLayer::MAIN_PASS | RenderLayer::SHADOW_CASTER) );
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
			Shader* shader = shaderManager->get(subMesh.material.getShaderName());
			if (!shader) continue;

			shaderBucket[shader].emplace_back(light->lightMesh.get(), &subMesh, model, (RenderLayer::MAIN_PASS) );
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

		// Upload Environment Data
		UploadEnvironmentData(shader, scene);

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

void Renderer::ShadowPass(Scene* scene, float dt)
{
	glEnable(GL_DEPTH_TEST);
	Shader* shadowMapShader = shaderManager->get("shadowMap");;
	Shader* shadowCubemapShader = shaderManager->get("shadowCubemap");
	for (auto& light : scene->getLights()) {
		if (light && light->mShadowMap) {

			switch (light->mShadowMap->getShadowMapType()) {
				case ShadowMapType::DEPTH_TEXTURE:
					shadowMapShader->Activate();
					RenderDepthTextureShadow(scene, dt, light.get(), shadowMapShader);
					break;

				case ShadowMapType::DEPTH_CUBEMAP:
					shadowCubemapShader->Activate();
					GL_CHECK_ERROR();

					RenderDepthCubemapShadow(scene, dt, light.get(), shadowCubemapShader);
					break;
			}

		}
	}
}

void Renderer::RenderDepthTextureShadow(Scene* scene, float dt, Light* light, Shader* shader)
{

	// Render to Texture Pass for Shadow Map
	light->mShadowMap->getShadowFBO().Bind();
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.1f, 4.0f);

	// Upload light data to Vertex Shader, once per shader
	shader->setUniformMat4fv("matrixShadow", light->getLightProjectionMatrix() * light->getLightViewMatrices()[0]);


	for (auto& [_, renderItems] : shaderBucket) {

		// Iterate through all assciated subMeshes
		for (auto& renderItem : renderItems) {
			if (!(renderItem.renderLayer & RenderLayer::SHADOW_CASTER)) continue;	// Avoid light mesh being able to cast shadows

			// Send model to Vertex Shader
			shader->setUniformMat4fv("modelMatrix", renderItem.modelMatrix);

			// Draw sub Mesh
			renderItem.meshRef->DrawSubMesh(*renderItem.subMeshRef);
		}

	}

	glDisable(GL_POLYGON_OFFSET_FILL);

	light->mShadowMap->getShadowFBO().Unbind();
}

void Renderer::RenderDepthCubemapShadow(Scene* scene, float dt, Light* light, Shader* shader)
{
	const glm::mat4 lightProj = light->getLightProjectionMatrix();
	const std::array<glm::mat4, 6>& lightViews = light->getLightViewMatrices();
	light->mShadowMap->getShadowFBO().Bind();
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.5f, 4.0f);
	GL_CHECK_ERROR();

	for (int faceIndex = 0; faceIndex < 6; ++faceIndex) {

		// Render to Texture Pass for Shadow Map
		light->mShadowMap->attachFace(faceIndex);
		GL_CHECK_ERROR();

		glClear(GL_DEPTH_BUFFER_BIT);

		// Upload light data to Vertex Shader, once per shader
		shader->setUniformMat4fv("matrixShadow", lightProj * lightViews[faceIndex]);
		light->sendLightFarPlaneToShader(*shader);
		light->sendLightLightPosToShader(*shader);


		for (auto& [_, renderItems] : shaderBucket) {

			// Iterate through all assciated subMeshes
			for (auto& renderItem : renderItems) {
				if (!(renderItem.renderLayer & RenderLayer::SHADOW_CASTER)) continue;	// Avoid light mesh being able to cast shadows

				// Send model to Vertex Shader
				shader->setUniformMat4fv("modelMatrix", renderItem.modelMatrix);

				// Draw sub Mesh
				renderItem.meshRef->DrawSubMesh(*renderItem.subMeshRef);
			}

		}
		GL_CHECK_ERROR();

	}
	glDisable(GL_POLYGON_OFFSET_FILL);
	light->mShadowMap->getShadowFBO().Unbind();
	GL_CHECK_ERROR();

}

void Renderer::DrawPass(Scene* scene, float dt)
{

	for (auto& [shader, renderItems] : shaderBucket) {

		if (!shader) {
			LogRendererWarn("Shader not found for " + shader->getVertexFile());
			continue;
		}

		// Use appropriate shader's to draw object
		shader->Activate();

		UploadCameraData(shader, scene);
		UploadEnvironmentData(shader, scene);

		// Upload light data to Vertex Shader, once per shader
		for (auto& light : scene->getLights()) {
			light->sendLightDataToShader(*shader);
			if (light->mShadowMap) light->mShadowMap->sendShadowData(*shader);
			light->sendShadowMatrices(*shader);
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

	Shader* envShader = shaderManager->get("skybox");

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

	// Upload Environment Data
	UploadEnvironmentData(envShader, scene);

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

	if (!RenderToTextureFBO || !RenderToTextureFBO->getColorTexture()) {
		LogRendererWarn("FBO not prepared for Post-Process Pass");
	}

	// Generate Mip maps for the rendered texture
	RenderToTextureFBO->getColorTexture()->Bind(textureSlots::RENDER_TEXTURE);
	glGenerateMipmap(GL_TEXTURE_2D);
	Shader* curShader = shaderManager->get("framebuffer");

	if (!curShader) {
		LogRendererWarn("Shader not found for framebuffer");
	}

	curShader->Activate();

	curShader->setSampler("screenTexture", textureSlots::RENDER_TEXTURE);
	RenderToTextureFBO->getColorTexture()->Bind(textureSlots::RENDER_TEXTURE);

	UploadCameraData(curShader, scene);

	// Send model to Vertex Shader
	glm::mat4 model = scene->getQuadController().getMesh()->computeModelMatrix();
	curShader->setUniformMat4fv("modelMatrix", model);

	for (auto subMesh : scene->getQuadController().getMesh()->getSubMeshes()) {
		scene->getQuadController().getMesh()->DrawSubMesh(subMesh);
	}
}

void Renderer::TempShadowMapRenderToQuadPass(Scene* scene)
{
	for (auto& light : scene->getLights()) {
		if (!light->mShadowMap || !light->mShadowMap->getShadowFBO().getDepthTexture()) {
			LogRendererWarn("FBO not prepared for TempShadowMapRenderToQuadPass");
		}

		// Generate Mip maps for the rendered texture
		light->mShadowMap->getShadowFBO().getDepthTexture()->Bind(textureSlots::RENDER_TEXTURE);
		//glGenerateMipmap(GL_TEXTURE_2D);
		Shader* curShader = shaderManager->get("framebuffer");

		if (!curShader) {
			LogRendererWarn("Shader not found for framebuffer");
		}
		
		curShader->Activate();

		curShader->setSampler("screenTexture", textureSlots::RENDER_TEXTURE);
		light->mShadowMap->getShadowFBO().getDepthTexture()->Bind(textureSlots::RENDER_TEXTURE);

		UploadCameraData(curShader, scene);

		// Send model to Vertex Shader
		glm::mat4 model = scene->getQuadController().getMesh()->computeModelMatrix();
		curShader->setUniformMat4fv("modelMatrix", model);

		for (auto subMesh : scene->getQuadController().getMesh()->getSubMeshes()) {
			scene->getQuadController().getMesh()->DrawSubMesh(subMesh);
		}
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

	ShadowPass(scene, dt);


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
	if (!ReflectionFBO || !ReflectionFBO->getColorTexture()) {
		LogRendererWarn("FBO not prepared for Draw Plane with Shader Pass");
	}

	// Generate Mip maps for the rendered texture
	ReflectionFBO->getColorTexture()->Bind(textureSlots::RENDER_TEXTURE);
	glGenerateMipmap(GL_TEXTURE_2D);

	Shader* planeReflectionShader = shaderManager->get("planeReflection");

	if (!planeReflectionShader) {
		LogRendererWarn("Shader not found for plane reflections");
	}
	planeReflectionShader->Activate();

	planeReflectionShader->setSampler("screenTexture", textureSlots::RENDER_TEXTURE);
	ReflectionFBO->getColorTexture()->Bind(textureSlots::RENDER_TEXTURE);

	// Upload Environment Data
	UploadEnvironmentData(planeReflectionShader, scene);
	GL_CHECK_ERROR();


	// Upload env light intensity and specular lighting to Vertex Shader
	for (auto& light : scene->getLights()) {
		if (light->dirty) light->validate();
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
	FBODescriptor desc;

	TextureDescriptor colorAttachment;
	colorAttachment.width = 4096;
	colorAttachment.height = 4096;
	colorAttachment.format = GL_RGB;
	colorAttachment.internalFormat = GL_RGBA8;
	colorAttachment.pixelType = GL_UNSIGNED_BYTE;
	colorAttachment.target = GL_TEXTURE_2D;

	// Create Tex Parameters list
	colorAttachment.parameters =
	{
		// Set texture parameters so we use bi-linear interpolation
		{GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},	// To avoid "dancing pixels"
		{GL_TEXTURE_MAG_FILTER, GL_LINEAR},
		// How texture coordinates are handled outside of region 0,0 to 1,1
		{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
		{GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
	};

	RenderBufferDescriptor renderBufferDesc;

	renderBufferDesc.height = 4096;
	renderBufferDesc.width = 4096;
	renderBufferDesc.internalFormat = GL_DEPTH_COMPONENT;
	renderBufferDesc.target = GL_RENDERBUFFER;

	desc.ColorAttachmentDesc = colorAttachment;
	desc.DepthRenderbufferDesc = renderBufferDesc;
	desc.DepthTextureDesc = std::nullopt;

	RenderToTextureFBO = std::make_unique<FBO>(4096, 4096, w, h); // use higher resolution texture width and height to allow for better magnification
	RenderToTextureFBO->Construct(desc);
}

void Renderer::createReflectionFBO()
{
	auto [w, h] = window.getWindowDimensions();
	FBODescriptor desc;

	TextureDescriptor colorAttachment;
	colorAttachment.width = engineConfig::REFLECTION_RESOLUTION_WIDTH;
	colorAttachment.height = engineConfig::REFLECTION_RESOLUTION_WIDTH;
	colorAttachment.format = GL_RGB;
	colorAttachment.internalFormat = GL_RGBA8;
	colorAttachment.pixelType = GL_UNSIGNED_BYTE;
	colorAttachment.target = GL_TEXTURE_2D;

	// Create Tex Parameters list
	colorAttachment.parameters =
	{
		// Set texture parameters so we use bi-linear interpolation
		{GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},	// To avoid "dancing pixels"
		{GL_TEXTURE_MAG_FILTER, GL_LINEAR},
		// How texture coordinates are handled outside of region 0,0 to 1,1
		{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
		{GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
	};

	RenderBufferDescriptor renderBufferDesc;

	renderBufferDesc.height = engineConfig::REFLECTION_RESOLUTION_WIDTH;
	renderBufferDesc.width = engineConfig::REFLECTION_RESOLUTION_WIDTH;
	renderBufferDesc.internalFormat = GL_DEPTH_COMPONENT;
	renderBufferDesc.target = GL_RENDERBUFFER;

	desc.ColorAttachmentDesc = colorAttachment;
	desc.DepthRenderbufferDesc = renderBufferDesc;
	desc.DepthTextureDesc = std::nullopt;

	ReflectionFBO = std::make_unique<FBO>(engineConfig::REFLECTION_RESOLUTION_WIDTH, engineConfig::REFLECTION_RESOLUTION_HEIGHT, w, h);
	ReflectionFBO->Construct(desc);
}

void Renderer::createEnvMapBackgroundObject()
{
	cubeMapMesh = std::make_unique<MeshComponent>("OBJ/Triangle/triangle.obj", false);
	cubeMapMesh->getSubMeshes();
	for (auto& submesh : cubeMapMesh->getSubMeshes()) {
		submesh.material.setShaderName("skybox");
	}

}

void Renderer::UploadEnvironmentData(Shader* shader, Scene* scene)
{
	// Upload CubeMap
	shader->setSampler("env", textureSlots::CUBE_MAP);
	scene->getCubeMap().Bind(textureSlots::CUBE_MAP);

	shader->setUniform1f("envLightIntensity", scene->getEnvLightIntensity());
}

void Renderer::UploadCameraData(Shader* shader, Scene* scene)
{
	// Send needed uniforms to shader
	scene->getCamera().sendViewAndProjToShader(*shader);
	scene->getCamera().sendMirroredViewToShader(*shader);
	scene->getCamera().sendCamPositionWorldSpaceToShader(*shader);
}
