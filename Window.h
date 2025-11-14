#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"InputManager.h"
#include"ShaderManager.h"

class Window {

	int windowHeight;
	int windowWidth;
	std::string winName;
	GLFWwindow* window;

public:

	Window(int height, int width, std::string name);
	bool initialize();
	bool shouldClose();
	void swapBuffers();
	void update(float deltaTime);
	void pollEvents();
	void terminateWindow();
	void enableDepthTest();

	// Getters and setters
	GLFWwindow* getGLFWwindow();

};