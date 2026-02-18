#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>

#include"EngineConfig.h"
#include"Window.h"
#include"MeshComponent.h"
#include"Vertex.h"
#include"Renderer.h"
#include"Camera.h"
#include"Light.h"
#include "SceneException.h"
#include "CubeMap.h"
#include "TextureManager.h"

int main() {

	// Initialize Window, ShaderManager, InputManager and Renderer

	Window window(engineConfig::DEFAULT_HEIGHT, engineConfig::DEFAULT_WIDTH, "PK_Engine");
	if (!window.initialize()) return -1;
	window.registerCallbacks();

	shared_ptr<InputManager> inputManager = make_shared<InputManager>(window.getGLFWwindow());

	// Lambda Function, basically just inlining a function to match the dispatcher's signature
	// we "capture" the inputManager so that we know which object isntance to use
	// using RAII to avoid trying to call function on deleted object
	inputManager->keyListenerToken = window.keyCallbackDispatcher.registerListener(
		[weakPtr = weak_ptr<InputManager>(inputManager)](int key, int scancode, int action, int mods) {
			if (auto p = weakPtr.lock()) p->handleKeyCallback(key, scancode, action, mods);
		}	
	);

	inputManager->cursorPosListenerToken = window.cursorPosCallbackDispatcher.registerListener(
		[weakPtr = weak_ptr<InputManager>(inputManager)](double xpos, double ypos) {
			if (auto p = weakPtr.lock()) p->handleCursorPosCallback(xpos, ypos);
		}
	);

	inputManager->mouseButtonListenerToken = window.mouseButtonCallbackDispatcher.registerListener(
		[weakPtr = weak_ptr<InputManager>(inputManager)](int button, int action, int mods) {
			if (auto p = weakPtr.lock()) p->handleMouseButtonCallback(button, action, mods);
		}
	);

	ShaderManager shaderManager;
	TextureManager textureManager;
	Renderer renderer(window, shaderManager);


	// Tmporary for testing --------------------------------

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
	MeshComponent rectangle(name, verts, ind);	
	MeshComponent sphere("OBJ/sphere/sphere.obj");
	//MeshComponent teapot("OBJ/teapot_white/teapot.obj");
	//MeshComponent yoda("OBJ/yoda/yoda.obj");

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

	// Transform rectangle
	Transform tR = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	};
	//rectangle.setTransform(tR);
	
	// STRESS TEST OBJ READER
	//MeshComponent cubeParseTest("square.obj");
	//MeshComponent mixed_indices("stress_test_obj/mixed_indices.obj");
	//MeshComponent no_normals("stress_test_obj/no_normals.obj");
	//MeshComponent quad_face("stress_test_obj/quad_face.obj");
	//MeshComponent redundant_vertices("stress_test_obj/redundant_vertices.obj");
	//MeshComponent shared_position_diff_normals("stress_test_obj/shared_position_diff_normals.obj");
	//MeshComponent triangle_basic("stress_test_obj/triangle_basic.obj");
	//MeshComponent triangle_mismatched_normals("stress_test_obj/triangle_mismatched_normals.obj");

	// TEMPORARY LIGHTS
	Light light;
	light.color = glm::vec3(1.0f,1.0f,1.0f);
	light.position = glm::vec3(3, 0, 3);
	light.ambient = glm::vec3(0.6f);
	light.diffuse = glm::vec3(1.0f); // darken diffuse light a bit
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);


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
	
	// load default vertex shader and fragement shader into shaderManager
	shaderManager.load("default", "default.vert", "default.frag");
	shaderManager.load("framebuffer", "framebuffer.vert", "framebuffer.frag");
	shaderManager.load("skybox", "skybox.vert", "skybox.frag");

	// Create Scene
	Scene basic;
	basic.AddMesh(&sphere);
	basic.Addlight(&light);
	basic.setCamera(std::make_unique<Camera>(engineConfig::DEFAULT_HEIGHT, engineConfig::DEFAULT_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f), 45.0f, 0.1f, 100.0f));
	basic.setQuadController(std::make_unique<QuadController>(&rectangle));
	basic.setCubeMap(std::make_unique<CubeMap>(paths));

	// Validate Scene, Return early if scene's are not created properly
	try {
		basic.validate();
	}
	catch (const SceneException& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	
	// deltaTime setup
	float prevTime = glfwGetTime();
	float deltaTime = 0.0f;
	float currentTime = 0.0f;

	while (!window.shouldClose()) {

		// Getting Delta Time
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		window.update(deltaTime);


		// Background color, clear alpha channel so that rendered texture with object its placed on allows for mixing colors
		glClearColor(0.0f, 0.f, 0.f, 0.0f);

		// for now handling application specific events in main later move to application or engine class

		if (inputManager->isKeyPressed(GLFW_KEY_ESCAPE)) {
			std::cout << "ESC CALLED" << std::endl;
			glfwSetWindowShouldClose(window.getGLFWwindow(), GLFW_TRUE);
		}

		if (inputManager->isKeyPressed(GLFW_KEY_F6)) {
			std::cout << "RELOAD SHADERS CALLED" << std::endl;
			shaderManager.reloadAll();
		}

		renderer.RenderFrame(&basic, inputManager, deltaTime);

		inputManager->update(deltaTime);
	}
	window.terminateWindow();
	return 0;
}