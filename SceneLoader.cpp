#include "Scene.h"
#include "SceneLoader.h"
#include "MeshFactory.h"
#include "Window.h"

SceneLoader::SceneLoader(Project::ProjectNumber project) : project(project){}

std::unique_ptr<Scene> SceneLoader::createBasicScene()
{
	Window* win = Application::Get().getWindow();
	std::unique_ptr<Scene> basic = std::make_unique<Scene>();

	//std::unique_ptr<MeshComponent> yoda = std::make_unique<MeshComponent>("OBJ/yoda/yoda.obj");


	// Enviornment CUBEMAP paths, MUST initialize array in proper order, +X, -X, +Y, -Y, +Z, -Z
	std::array<std::string, 6> paths = {
		"envMaps/cubemap/cubemap_posx.png",
		"envMaps/cubemap/cubemap_negx.png",
		"envMaps/cubemap/cubemap_posy.png",
		"envMaps/cubemap/cubemap_negy.png",
		"envMaps/cubemap/cubemap_posz.png",
		"envMaps/cubemap/cubemap_negz.png",
	};

	// Create Scene

	createProject(project, basic.get());

	// CHANGE LIGHT HERE
	basic->Addlight(move(createLight(LightType::POINT, win)));
	basic->setCamera(std::make_unique<Camera>(engineConfig::DEFAULT_WIDTH, engineConfig::DEFAULT_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f), 45.0f, 0.1f, 100.0f));
	basic->setLightController(std::make_unique<LightController>());
	basic->setCubeMap(std::make_unique<CubeMap>(paths));
	basic->setEnvLightIntensity(0.8f);

	// Validate Scene, Return early if scene's are not created properly
	try {
		basic->validate();
	}
	catch (const SceneException& e) {
		std::cerr << e.what() << std::endl;
		return nullptr;
	}

	return basic;
}

std::unique_ptr<Light> SceneLoader::createLight(LightType type, Window* win)
{
	// light Mesh
	std::unique_ptr<MeshComponent> lightMesh = std::make_unique<MeshComponent>("OBJ/light/light.obj");
	Transform tRLightMesh;
	tRLightMesh.translation = glm::vec3(0.0f, 0.4f, 0.0f);
	tRLightMesh.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	tRLightMesh.scale = glm::vec3(0.25f, 0.25f, 0.25f);
	lightMesh->setTransform(tRLightMesh);

	// Shadow Map
	auto [w, h] = win->getWindowDimensions();

	std::unique_ptr<ShadowMap> shadowMap = std::make_unique<ShadowMap>(1024, 1024);

	auto light = std::make_unique<Light>();

	light->color = glm::vec3(1.0f);
	light->diffuse = glm::vec3(1.0f);
	light->specular = glm::vec3(1.0f);
	light->lightMesh = std::move(lightMesh);
	light->mShadowMap = std::move(shadowMap);

	switch (type) {
		case LightType::POINT:

			// POINT LIGHT
			light->lightType = LightType::POINT;
			light->position = glm::vec3(1, 0, 1);
			light->ambient = glm::vec3(0.6f);
			light->shouldShowMesh = true;
			light->mShadowMap->createDepthCubeMapFBO(w, h); // How high quality shadow res is depends on tex width & height (I think)
			break;

		case LightType::DIRECTIONAL:
			// DIRECTIONAL 
			light->lightType = LightType::DIRECTIONAL;
			light->ambient = glm::vec3(0.6f);
			light->shouldShowMesh = false;
			light->mShadowMap->createDepthMapFBO(w, h);
			break;

		case LightType::SPOT:

			// SPOT LIGHT 
			light->lightType = LightType::SPOT;
			light->ambient = glm::vec3(0.9f);
			light->position = glm::vec3(0, 0.8f, 0);
			light->innerCone = glm::cos(glm::radians(12.5f));
			light->outerCone = glm::cos(glm::radians(40.5f));
			light->shouldShowMesh = true;
			light->mShadowMap->createDepthMapFBO(w, h);
			break;

		default:
			std::cerr << "[SCENELOADER] Unknown light type" << std::endl;
			return nullptr;

	}

	return light;

}

void SceneLoader::createProject(Project::ProjectNumber proj, Scene* scene)
{
	// Vertices coordinates for Rectangle
	VERTEX vertices[] =
	{ //               COORDINATES           /
		VERTEX{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},	// bottom left
		VERTEX{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},	// bottom right
		VERTEX{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},	// top right
		VERTEX{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},	// top left
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2, // Lower left triangle
		0, 3, 2, // upper right triangle
	};
	std::string name = "rectangle";
	std::vector<VERTEX> verts(vertices, vertices + sizeof(vertices) / sizeof(VERTEX));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::unique_ptr<MeshComponent> rectangle = std::make_unique<MeshComponent>(name, verts, ind);

	std::unique_ptr<MeshComponent> teapot = std::make_unique<MeshComponent>("OBJ/teapot/teapot.obj");
	std::unique_ptr<MeshComponent> teapot_facing_up = std::make_unique<MeshComponent>("OBJ/teapot_facing_up/teapot.obj");
	std::unique_ptr<MeshComponent> planeMesh = std::make_unique<MeshComponent>("OBJ/plane/plane.obj");

	std::unique_ptr<MeshComponent> planeMeshTriangles = std::make_unique<MeshComponent>("OBJ/plane/plane.obj");
	std::unique_ptr<MeshFactory> meshFactory = std::make_unique<MeshFactory>();
	std::unique_ptr<MeshComponent> planeMeshQuads = meshFactory.get()->CreateQuadGrid(1);

	glm::quat rotation_quat = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Transform tR;


	switch (proj)
	{
	case Project::ProjectNumber::PROJECT5:

		// Enable Env in EngineConfig.h

		renderSettings::USING_ENV = true;
		renderSettings::USING_RENDER_TO_TEXTURE_SIMPLE = true;
		renderSettings::USING_SHADOWS = false;
		renderSettings::USING_TESSELLATION = false;
		renderSettings::USING_NORMALMAPPING = false;
		renderSettings::USING_REFLECTIONS = false;

		tR.translation = glm::vec3(0.0f, 0.0f, 0.0f);
		tR.rotation = rotation_quat;
		tR.scale = glm::vec3(1.0f, 1.0f, 1.0f);

		teapot->setTransform(tR);
		scene->setQuadController(std::make_unique<QuadController>(move(rectangle)));
		scene->AddMesh(move(teapot));

		break;	
	case Project::ProjectNumber::PROJECT6:

		// Enable Reflections, Env in EngineConfig.h
		// Choose any type of light source
		renderSettings::USING_ENV = true;
		renderSettings::USING_RENDER_TO_TEXTURE_SIMPLE = false;
		renderSettings::USING_SHADOWS = false;
		renderSettings::USING_TESSELLATION = false;
		renderSettings::USING_NORMALMAPPING = false;
		renderSettings::USING_REFLECTIONS = true;

		scene->AddMesh(move(teapot_facing_up));
		scene->createMirrorObject(glm::vec3(0.0f, -0.18f, 0.0f));

		break;	
	case Project::ProjectNumber::PROJECT7:

		// Enable Shadows, Env in EngineConfig.h
		// Choose any type of light source
		renderSettings::USING_ENV = true;
		renderSettings::USING_RENDER_TO_TEXTURE_SIMPLE = false;
		renderSettings::USING_SHADOWS = true;
		renderSettings::USING_TESSELLATION = false;
		renderSettings::USING_NORMALMAPPING = false;
		renderSettings::USING_REFLECTIONS = false;

		tR.translation = glm::vec3(0.0f, -0.18f, 0.0f);
		tR.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		tR.scale = glm::vec3(3.0f, 1.0f, 3.0f);
		planeMesh->setTransform(tR);

		tR.translation = glm::vec3(0.0f, 0.0f, 0.0f);
		tR.rotation = rotation_quat;
		tR.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		teapot->setTransform(tR);

		scene->AddMesh(move(teapot));
		scene->AddMesh(move(planeMesh));


		break;	
	case Project::ProjectNumber::PROJECT8:
		// Enable Tessellation, Shadows, Normal Mapping, Env in EngineConfig.h
		// Choose any type of light source
		renderSettings::USING_ENV = true;
		renderSettings::USING_RENDER_TO_TEXTURE_SIMPLE = false;
		renderSettings::USING_SHADOWS = true;
		renderSettings::USING_TESSELLATION = true;
		renderSettings::USING_NORMALMAPPING = true;
		renderSettings::USING_REFLECTIONS = false;

		// Change Normal & Displacement Maps here 
		planeMeshQuads->getSubMeshes()[0].material.setNormalMapPath("normalMaps/broken_brick_wall_nor_gl_4k.png");
		planeMeshTriangles->getSubMeshes()[0].material.setNormalMapPath("normalMaps/teapot_normal.png");
		planeMeshTriangles->getSubMeshes()[0].material.setDisplacementMapPath("displacementMaps/teapot_disp.png");
		planeMeshQuads->getSubMeshes()[0].material.setDisplacementMapPath("displacementMaps/broken_brick_wall_disp_4k.png");

		// Transform plane mesh
		tR.translation = glm::vec3(0.0f, -0.18f, 0.0f);
		tR.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		tR.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		planeMeshQuads->setTransform(tR);
		//planeMeshTriangles->setTransform(tRProject8);

		 scene->AddMesh(move(planeMeshQuads));

		break;
	}
}
