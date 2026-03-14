#include "Application.h"
#include"EngineConfig.h"
#include"Window.h"
#include"MeshComponent.h"
#include"Vertex.h"
#include"Renderer.h"
#include"Camera.h"
#include"Light.h"
#include "SceneException.h"
#include "CubeMap.h"
#include "TextureManager.h"
#include "SceneLoader.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	s_Instance = this;	// To be able to get Application 
}

Application::~Application()
{
	s_Instance = nullptr;
}

bool Application::Init()
{
	// Initialize Window, ShaderManager, InputManager and Renderer

	m_Window = std::make_unique<Window>(engineConfig::DEFAULT_HEIGHT, engineConfig::DEFAULT_WIDTH, "PK_Engine");
	if (!m_Window->initialize()) {
		std::cerr << "[OpenGL] Error Initilizing Window \n";
		return false;
	}
	m_Window->registerCallbacks();

	m_InputManager = std::make_shared<InputManager>(m_Window->getGLFWwindow());

	// Lambda Function, basically just inlining a function to match the dispatcher's signature
	// we "capture" the inputManager so that we know which object isntance to use
	// using RAII to avoid trying to call function on deleted object
	m_InputManager->keyListenerToken = m_Window->keyCallbackDispatcher.registerListener(
		[weakPtr = std::weak_ptr<InputManager>(m_InputManager)](int key, int scancode, int action, int mods) {
			if (auto p = weakPtr.lock()) p->handleKeyCallback(key, scancode, action, mods);
		}
	);

	m_InputManager->cursorPosListenerToken = m_Window->cursorPosCallbackDispatcher.registerListener(
		[weakPtr = std::weak_ptr<InputManager>(m_InputManager)](double xpos, double ypos) {
			if (auto p = weakPtr.lock()) p->handleCursorPosCallback(xpos, ypos);
		}
	);

	m_InputManager->mouseButtonListenerToken = m_Window->mouseButtonCallbackDispatcher.registerListener(
		[weakPtr = std::weak_ptr<InputManager>(m_InputManager)](int button, int action, int mods) {
			if (auto p = weakPtr.lock()) p->handleMouseButtonCallback(button, action, mods);
		}
	);

	m_ShaderManager = std::make_unique<ShaderManager>();
	m_TextureManager = std::make_unique<TextureManager>();
	m_TextureManager->initFallback();
	m_Renderer = std::make_unique<Renderer>(*m_Window);

	// load default vertex shader and fragement shader into shaderManager
	m_ShaderManager->load("default", "default.vert", "default.frag");
	m_ShaderManager->load("framebuffer", "framebuffer.vert", "framebuffer.frag");
	m_ShaderManager->load("skybox", "skybox.vert", "skybox.frag");
	m_ShaderManager->load("planeReflection", "planeReflection.vert", "planeReflection.frag");

	// Scene Loader, create basic scene
	SceneLoader loader;
	m_Scene = loader.createBasicScene();

	if (!m_Scene) return false;

	return true;
}

void Application::Run()
{
	// deltaTime setup
	float prevTime = glfwGetTime();

	while (!m_Window->shouldClose()) {

		// Getting Delta Time
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		m_Window->update(deltaTime);
		s_Instance->handleInput();

		if (m_Scene) m_Renderer->RenderFrame(m_Scene.get(), deltaTime);	// For NOW use one scene

		m_InputManager->update(deltaTime);
	}
}

void Application::Stop()
{
}

void Application::handleInput()
{
	if (m_InputManager->isKeyPressed(GLFW_KEY_ESCAPE)) {
		std::cout << "ESC CALLED" << std::endl;
		glfwSetWindowShouldClose(m_Window->getGLFWwindow(), GLFW_TRUE);
	}

	if (m_InputManager->isKeyPressed(GLFW_KEY_F6)) {
		std::cout << "RELOAD SHADERS CALLED" << std::endl;
		m_ShaderManager->reloadAll();
	}
}

Application& Application::Get()
{
	return *Application::s_Instance;
}

Window* Application::getWindow()
{
	return m_Window.get();
}

ShaderManager* Application::getShaderManager()
{
	return m_ShaderManager.get();
}

TextureManager* Application::getTextureManager()
{
	return m_TextureManager.get();
}

Renderer* Application::getRenderer()
{
	return m_Renderer.get();
}

InputManager* Application::getInputManager()
{
	return m_InputManager.get();
}
