#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"InputManager.h"

class Window {

	int windowHeight;
	int windowWidth;
	std::string winName;
	GLFWwindow* window;
	InputManager* inputManager;	// will have multiple input Managers per window


public:

	Window(int height, int width, std::string name);
	bool initialize();
	bool shouldClose();
	void swapBuffers();
	void pollEvents();
	void terminateWindow();

	// input handling, calling appropriate functions, must be static to not directly reference THIS instance of the window since GLFW doesn't now what "THIS" is
	static void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);


};