#include "InputManager.h"

void InputManager::handleKeyCallback(int key, int scancode, int action, int mods)
{
	// if key lacks a token for it
	if (key == GLFW_KEY_UNKNOWN) {
		return;
	}

	// Don't need to erase KeyPressed or KeyReleased as that is taken care of in update()
	if (action == GLFW_PRESS) {
		keyPressedThisFrame.insert(key);
		keyHeld.insert(key);
	}
	else if (action == GLFW_RELEASE) {

		keyReleasedThisFrame.insert(key);
		keyHeld.erase(key);
	}
	else if (action != GLFW_REPEAT) {
		LogInputManagerError("Unknown key action: " + to_string(action));
	}
}

void InputManager::handleCursorPosCallback(double xpos, double ypos)
{
	if (xpos < 0 || ypos < 0) return; // Prevent erratic camera jumps if window suddenly loses focus

	// First update delta's before updating previous frame variables
	deltaMousePosX = xpos - mousePosX;
	deltaMousePosY = ypos - mousePosY;

	lastMousePosX = mousePosX;
	lastMousePosY = mousePosY;

	mousePosX = xpos;
	mousePosY = ypos;
}

void InputManager::handleMouseButtonCallback(int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		mouseButtonPressedThisFrame.insert(button);
		mouseButtonHeld.insert(button);
	}
	else if (action == GLFW_RELEASE) {
		mouseButtonReleasedThisFrame.insert(button);
		mouseButtonHeld.erase(button);
	}

}

InputManager::InputManager(GLFWwindow* window)
{
	this->window = window;

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
