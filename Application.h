#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<vector>

class Renderer;
class TextureManager;
class ShaderManager;
class InputManager;
class Window;
class Scene;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	void Run();
	void Stop();
	void handleInput();

	static Application& Get();
	Window* getWindow();
	ShaderManager* getShaderManager();
	TextureManager* getTextureManager();
	Renderer* getRenderer();
	InputManager* getInputManager();

private:

	// Store all as unique_ptr rather than object to avoid immediate construction and 
	// to allow for sole ownership
	std::unique_ptr<Window> m_Window;	// Must initialize Window BEFORE Renderer as Renderer uses window as a referenced object
	std::unique_ptr<ShaderManager> m_ShaderManager;
	std::unique_ptr<TextureManager> m_TextureManager;
	std::unique_ptr<Renderer> m_Renderer;
	std::shared_ptr<InputManager> m_InputManager;
	std::unique_ptr<Scene> m_Scene;

	static Application* s_Instance;

};