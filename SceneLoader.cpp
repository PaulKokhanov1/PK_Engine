#include "Scene.h"
#include "SceneLoader.h"

SceneLoader::SceneLoader()
{
}

SceneLoader::~SceneLoader()
{
}

std::unique_ptr<Scene> SceneLoader::createBasicScene()
{

	// Tmporary for testing ---------------------------------------------------------------------------------------------------

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
	//unique_ptr<MeshComponent> sphere = make_unique<MeshComponent>("OBJ/sphere/sphere.obj");
	std::unique_ptr<MeshComponent> teapot = std::make_unique<MeshComponent>("OBJ/teapot_facing_up/teapot.obj");
	//std::unique_ptr<MeshComponent> teapot = std::make_unique<MeshComponent>("OBJ/teapot/teapot.obj");
	//MeshComponent teapot("OBJ/teapot_white/teapot.obj");
	//std::unique_ptr<MeshComponent> yoda = std::make_unique<MeshComponent>("OBJ/yoda/yoda.obj");
	std::unique_ptr<MeshComponent> planeMesh = std::make_unique<MeshComponent>("OBJ/plane/plane.obj");
	// Transform plane mesh, ADD FUNCTIONALITY TO CHANGE TRANSFORM
	Transform tR;
	tR.translation = glm::vec3(0.0f, -0.18f, 0.0f);
	tR.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	tR.scale = glm::vec3(3.0f, 1.0f, 3.0f);
	planeMesh->setTransform(tR);

	// light Mesh
	std::unique_ptr<MeshComponent> lightMesh = std::make_unique<MeshComponent>("OBJ/light/light.obj");
	Transform tRLightMesh;
	tRLightMesh.translation = glm::vec3(0.0f, 0.4f, 0.0f);
	tRLightMesh.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	tRLightMesh.scale = glm::vec3(0.25f, 0.25f, 0.25f);
	lightMesh->setTransform(tRLightMesh);

	// Rotate Yoda
	// Define the rotation angle (90 degrees) and axis (Y-axis)
	float angle_degrees = -90.0f;
	glm::vec3 rotation_axis(1.0f, 0.0f, 0.0f); // Rotate around the Y-axis

	// Create the quaternion
	glm::quat rotation_quat = glm::angleAxis(glm::radians(angle_degrees), rotation_axis);
	Transform t = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		rotation_quat,
		glm::vec3(2.0f, 2.0f, 2.0f)
	};
	//yoda.setTransform(t);
	//teapot.setTransform(t);

	// POINT LIGHT
	std::unique_ptr<Light> lightPoint = std::make_unique<Light>();
	lightPoint->lightType = LightType::POINT;
	lightPoint->color = glm::vec3(1.0f, 1.0f, 1.0f);
	lightPoint->position = glm::vec3(1, 0, 1);
	lightPoint->ambient = glm::vec3(0.6f);
	lightPoint->diffuse = glm::vec3(1.0f);
	lightPoint->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	lightPoint->envLightIntensity = 0.8f;
	lightPoint->shouldShowMesh = true;
	//lightPoint->lightMesh = std::move(lightMesh);

	// DIRECTIONAL 
	std::unique_ptr<Light> lightDirectional = std::make_unique<Light>();
	lightDirectional->lightType = LightType::DIRECTIONAL;
	lightDirectional->color = glm::vec3(1.0f, 1.0f, 1.0f);
	lightDirectional->ambient = glm::vec3(0.6f);
	lightDirectional->diffuse = glm::vec3(1.0f);
	lightDirectional->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	lightDirectional->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	lightDirectional->envLightIntensity = 0.8f;
	lightDirectional->shouldShowMesh = false;

	// SPOT LIGHT 
	std::unique_ptr<Light> lightSpot = std::make_unique<Light>();
	lightSpot->lightType = LightType::SPOT;
	lightSpot->color = glm::vec3(1.0f, 1.0f, 1.0f);
	lightSpot->ambient = glm::vec3(0.9f);
	lightSpot->diffuse = glm::vec3(1.0f);
	lightSpot->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	lightSpot->position = glm::vec3(0, 0.8f, 0);
	lightSpot->direction = glm::vec3(0.0f, -1.0f, 0.0f);	// Add ability to move direction of directional light
	lightSpot->envLightIntensity = 0.8f;
	lightSpot->innerCone = glm::cos(glm::radians(12.5f));
	lightSpot->outerCone = glm::cos(glm::radians(17.5f));
	lightSpot->shouldShowMesh = true;
	lightSpot->lightMesh = std::move(lightMesh);


	// TEMPORARY CUBEMAP paths, MUST initialize array in proper order, +X, -X, +Y, -Y, +Z, -Z
	std::array<std::string, 6> paths = {
		"envMaps/cubemap/cubemap_posx.png",
		"envMaps/cubemap/cubemap_negx.png",
		"envMaps/cubemap/cubemap_posy.png",
		"envMaps/cubemap/cubemap_negy.png",
		"envMaps/cubemap/cubemap_posz.png",
		"envMaps/cubemap/cubemap_negz.png",
	};

	//------------------------------------------------------------------------------------------------------------

	// Create Scene
	std::unique_ptr<Scene> basic = std::make_unique<Scene>();
	basic->AddMesh(move(teapot));
	basic->AddMesh(move(planeMesh));	// Comment This OUT when checking Project 6 (it overlaps the mirrored plane)
	basic->createMirrorObject(glm::vec3(0.0f, -0.18f, 0.0f));
	//basic->Addlight(move(lightPoint));
	//basic->Addlight(move(lightDirectional));
	basic->Addlight(move(lightSpot));
	basic->setCamera(std::make_unique<Camera>(engineConfig::DEFAULT_WIDTH, engineConfig::DEFAULT_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f), 45.0f, 0.1f, 100.0f));
	basic->setQuadController(std::make_unique<QuadController>(move(rectangle)));
	basic->setLightController(std::make_unique<LightController>());
	basic->setCubeMap(std::make_unique<CubeMap>(paths));

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
