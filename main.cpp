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

int main() {

	// Initialize Window, ShaderManager, InputManager and Renderer

	Window window(engineConfig::DEFAULT_HEIGHT, engineConfig::DEFAULT_WIDTH, "PK_Engine");
	if (!window.initialize()) return -1;

	InputManager inputManager(window.getGLFWwindow());
	inputManager.registerCallbacks();

	ShaderManager shaderManager;
	Renderer renderer(window, shaderManager);


	// Tmporary for testing --------------------------------

	// Vertices coordinates
	VERTEX vertices[] =
	{ //               COORDINATES           /
		VERTEX{glm::vec3(-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f)},
		VERTEX{glm::vec3(0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f)},
		VERTEX{glm::vec3(0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f)},
		VERTEX{glm::vec3(-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f)},
		VERTEX{glm::vec3(0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f)},
		VERTEX{glm::vec3(0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f)},

	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Lower right triangle
		5, 4, 1 // Upper triangle
	};

	Shader shader("default.vert", "default.frag");
	std::string name = "triangle";
	std::vector<VERTEX> verts(vertices, vertices + sizeof(vertices) / sizeof(VERTEX));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	MeshComponent triangle(name, verts, ind);
	MeshComponent teapot("teapoasdt.obj");
	
	// STRESS TEST OBJ READER
	//MeshComponent cubeParseTest("square.obj");
	//MeshComponent mixed_indices("stress_test_obj/mixed_indices.obj");
	//MeshComponent no_normals("stress_test_obj/no_normals.obj");
	//MeshComponent quad_face("stress_test_obj/quad_face.obj");
	//MeshComponent redundant_vertices("stress_test_obj/redundant_vertices.obj");
	//MeshComponent shared_position_diff_normals("stress_test_obj/shared_position_diff_normals.obj");
	//MeshComponent triangle_basic("stress_test_obj/triangle_basic.obj");
	//MeshComponent triangle_mismatched_normals("stress_test_obj/triangle_mismatched_normals.obj");

	//------------------------------------------------------------------------------------------------------------
	
	// load default vertex shader and fragement shader into shaderManager
	shaderManager.load("default", "default.vert", "default.frag");

	// Create Scene
	Scene basic;
	basic.AddMesh(&teapot);
	basic.setCamera(std::make_unique<Camera>(engineConfig::DEFAULT_HEIGHT, engineConfig::DEFAULT_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f), 45.0f, 0.1f, 100.0f));
	
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


		// Background color
		glClearColor(0.0f, 0.f, 0.f, 1.0f);

		// for now handling application specific events in main later move to application or engine class

		if (inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
			std::cout << "ESC CALLED" << std::endl;
			glfwSetWindowShouldClose(window.getGLFWwindow(), GLFW_TRUE);
		}

		if (inputManager.isKeyPressed(GLFW_KEY_F6)) {
			std::cout << "RELOAD SHADERS CALLED" << std::endl;
			shaderManager.reloadAll();
		}

		renderer.RenderFrame(&basic, &inputManager, deltaTime);

		inputManager.update(deltaTime);
	}
	window.terminateWindow();
	return 0;
}