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

	glfwSetWindowUserPointer(window, this);

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

void Window::registerCallbacks()
{
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void Window::window_size_callback(GLFWwindow* window, int width, int height)
{
	// Get curreent instance of window and handle resize
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	win->onResize(width, height);
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!win) return;
	win->cursorPosCallbackDispatcher.notifyListeners(xpos, ypos);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!win) return;
	win->keyCallbackDispatcher.notifyListeners(key, scancode, action, mods);

}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!win) return;
	win->mouseButtonCallbackDispatcher.notifyListeners(button, action, mods);

}

GLFWwindow* Window::getGLFWwindow()
{
	return window;
}

pair<int, int> Window::getWindowDimensions()
{
	return { windowWidth, windowHeight };
}

void Window::onResize(int width, int height)
{

	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, width, height);
	needsResize = true;
}



