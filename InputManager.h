#ifndef INPUTMANAGER_CLASS_H
#define INPUTMANAGER_CLASS_H

#include<unordered_set>
#include<iostream>
#include<GLFW/glfw3.h>

#include "LogInputManager.h"

class InputManager
{
private:

	GLFWwindow* window; // non-owning, cant make it weak ptr since GLFW manages own memeory so cant make window instance a shared ptr

	// track keys that are currently pressed
	std::unordered_set<int> keyPressedThisFrame;
	std::unordered_set<int> keyReleasedThisFrame;
	std::unordered_set<int> keyHeld;

	// track mouse state
	std::unordered_set<int> mouseButtonPressedThisFrame;
	std::unordered_set<int> mouseButtonReleasedThisFrame;
	std::unordered_set<int> mouseButtonHeld;


	// track position of mouse
	double mousePosX, mousePosY;
	double lastMousePosX, lastMousePosY;
	double deltaMousePosX, deltaMousePosY;	// +ve means moved right or down

	// Setup key and mouse callbacks, will update internal sets
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


public:
	InputManager(GLFWwindow* window);
	~InputManager();

	void registerCallbacks();
	void clearInputFrameStates();

	// System loop
	void update(float deltaTime);

	// Setters
	void setInputMode(int mode, int value);
	void setCursorPos(double xpos, double ypos);

	// Query's
	bool isKeyPressed(int key) const;
	bool isKeyHeld(int key) const;
	bool isKeyReleased(int key) const;

	bool isMouseButtonPressed(int button) const;
	bool isMouseButtonHeld(int button) const;
	bool isMouseButtonReleased(int button) const;

	double getMouseX() const;
	double getMouseY() const;	
	
	double getLastMouseX() const;
	double getLastMouseY() const;

	double getDeltaMouseX() const;
	double getDeltaMouseY() const;

};


#endif // !INPUTMANAGER_CLASS_H
