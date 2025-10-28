#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"EngineConfig.h"
#include"Window.h"

int main() {

	Window window(engineConfig::DEFAULT_HEIGHT, engineConfig::DEFAULT_WIDTH, "PK_Engine");
	if (!window.initialize()) return -1;
	glClearColor(0.9f, 0.13f, 0.17f, 1.0f);

	while (!window.shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT);


		window.swapBuffers();
		window.pollEvents();
	}
	window.terminateWindow();
	return 0;
}