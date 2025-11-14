#include "InputManager.h"

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// if key lacks a token for it
	if (key == GLFW_KEY_UNKNOWN) {
		return;
	}

	// Must get InputManager through getUserPointer as callback's are declared static
	InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

	// Don't need to erase KeyPressed or KeyReleased as that is taken care of in update()
	if (action == GLFW_PRESS) {
		inputManager->keyPressedThisFrame.insert(key);
		inputManager->keyHeld.insert(key);
	} else if (action == GLFW_RELEASE) {

		inputManager->keyReleasedThisFrame.insert(key);
		inputManager->keyHeld.erase(key);
	}
	else if (action != GLFW_REPEAT) {
		std::cerr << "[InputManager] Unknown key action: " << action << "\n";
	}
}

void InputManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (xpos < 0 || ypos < 0) return; // Prevent erratic camera jumps if window suddenly loses focus

	InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

	// First update delta's before updating previous frame variables
	inputManager->deltaMousePosX = xpos - inputManager->mousePosX;
	inputManager->deltaMousePosY = ypos - inputManager->mousePosY;

	inputManager->lastMousePosX = inputManager->mousePosX;
	inputManager->lastMousePosY = inputManager->mousePosY;

	inputManager->mousePosX = xpos;
	inputManager->mousePosY = ypos;

}

void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		inputManager->mouseButtonPressedThisFrame.insert(button);
		inputManager->mouseButtonHeld.insert(button);
	}
	else if (action == GLFW_RELEASE) {

		inputManager->mouseButtonReleasedThisFrame.insert(button);
		inputManager->mouseButtonHeld.erase(button);
	}

}

InputManager::InputManager(GLFWwindow* window)
{
	this->window = window;

	// Callback funcs are static hence we dont know the exact instance of what we are editing, hence we set the user pointer to be able to later get the isntance of this inputManager
	glfwSetWindowUserPointer(window, this); 

	mousePosX = 0.0;
	mousePosY = 0.0;;
	lastMousePosX = 0.0;
	lastMousePosY = 0.0;
	deltaMousePosX = 0.0;
	deltaMousePosY = 0.0;

}

InputManager::~InputManager()
{
}

void InputManager::registerCallbacks()
{
	if (!window) {
		std::cerr << "[InputManager] Error: Cannot register callbacks, window is null.\n";
		return;
	}

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void InputManager::clearInputFrameStates()
{
	keyPressedThisFrame.clear();
	keyReleasedThisFrame.clear();

	mouseButtonPressedThisFrame.clear();
	mouseButtonReleasedThisFrame.clear();

	deltaMousePosX = 0.0;
	deltaMousePosY = 0.0;

}

void InputManager::update(float deltaTime)
{
	// Prevent stuck keys if window loses focus
	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		keyHeld.clear();
		mouseButtonHeld.clear();
	}

	clearInputFrameStates();
}

void InputManager::setInputMode(int mode, int value)
{
	glfwSetInputMode(window, mode, value);
}

void InputManager::setCursorPos(double xpos, double ypos)
{
	glfwSetCursorPos(window, xpos, ypos);
}

bool InputManager::isKeyPressed(int key) const
{
	return keyPressedThisFrame.find(key) != keyPressedThisFrame.end();
}

bool InputManager::isKeyHeld(int key) const
{
	return keyHeld.find(key) != keyHeld.end();;
}

bool InputManager::isKeyReleased(int key) const
{
	return keyReleasedThisFrame.find(key) != keyReleasedThisFrame.end();
}

bool InputManager::isMouseButtonPressed(int button) const
{
	return mouseButtonPressedThisFrame.find(button) != mouseButtonPressedThisFrame.end();
}

bool InputManager::isMouseButtonHeld(int button) const
{
	return mouseButtonHeld.find(button) != mouseButtonHeld.end();
}

bool InputManager::isMouseButtonReleased(int button) const
{
	return mouseButtonReleasedThisFrame.find(button) != mouseButtonReleasedThisFrame.end();
}

double InputManager::getMouseX() const
{
	return mousePosX;
}

double InputManager::getMouseY() const
{
	return mousePosY;
}

double InputManager::getLastMouseX() const
{
	return lastMousePosX;
}

double InputManager::getLastMouseY() const
{
	return lastMousePosY;
}

double InputManager::getDeltaMouseX() const
{
	return deltaMousePosX;
}

double InputManager::getDeltaMouseY() const
{
	return deltaMousePosY;
}
