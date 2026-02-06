#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"InputManager.h"
#include"ShaderManager.h"
#include"LogWindow.h"
#include"EventDispatcher.h"

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
	void registerCallbacks();

	// Callbacks
	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	// Getters and setters
	GLFWwindow* getGLFWwindow();
	pair<int, int> getWindowDimensions();

	void onResize(int width, int height);

	bool needsResize = false;

	// Listeners
	EventDispatcher<int, int, int, int> keyCallbackDispatcher;
	EventDispatcher<double, double> cursorPosCallbackDispatcher;
	EventDispatcher<int, int, int> mouseButtonCallbackDispatcher;
	EventDispatcher<int, int> windowResizeCallbackDispatcher;

};