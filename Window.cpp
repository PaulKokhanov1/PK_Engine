#include"Window.h"

Window::Window(int height, int width, std::string name)
{
	windowHeight = height;
	windowWidth = width;
	winName = name;
	window = nullptr;
	inputManager = nullptr;

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
		std::cout << "Failed to Create Window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	// Load glad to congufure OpenGL
	gladLoadGL();

	glViewport(0, 0, windowWidth, windowHeight);

	// Create InputManger variable
	inputManager = new InputManager(window);

	inputManager->registerCallbacks();

	// Initialize key callbacks
	glfwSetKeyCallback(window, Window::onKeyEvent);

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

void Window::onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		std::cout << "ESC CALLED" << std::endl;
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

