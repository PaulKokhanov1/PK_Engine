#include"Window.h"

Window::Window(int height, int width, std::string name)
{
	windowHeight = height;
	windowWidth = width;
	winName = name;
	window = nullptr;
}

bool Window::initialize()
{
	// Ensure glfwInit() calls properly, avoid try catch because glfw doesn't throw exceptions, potentitaly LATER CREATE ErrorManager Class
	if (!glfwInit()) return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Attempt to create GLFWwindow object
	window = glfwCreateWindow(windowWidth, windowHeight, winName.c_str(), NULL, NULL);
	if (!window) {
		LogWindowError("Failed to Create Window");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	// Load glad to congufure OpenGL
	gladLoadGL();

	glViewport(0, 0, windowWidth, windowHeight);
	enableDepthTest();

	return true;
}

bool Window::shouldClose() 
{
	return glfwWindowShouldClose(window);
}

void Window::swapBuffers()
{
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
}

void Window::update(float deltaTime)
{
	pollEvents();
}

void Window::pollEvents()
{
	// Take care of all GLFW events
	glfwPollEvents();
}

void Window::terminateWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

GLFWwindow* Window::getGLFWwindow()
{
	return window;
}



