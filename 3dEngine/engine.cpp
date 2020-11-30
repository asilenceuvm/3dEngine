#include "engine.h"

Engine::Engine(int width, int height) {
	this->width = width;
	this->height = height;

	glewInit();

	//global gl settings
	//depth buffer
	glEnable(GL_DEPTH_TEST);
	
	//alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	
	//Anti Aliasing
	glfwWindowHint(GLFW_SAMPLES, 4); //4x MSAA
	glEnable(GL_MULTISAMPLE);
	
	//vsync
	//glfwSwapInterval(1); //vsync
	glfwSwapInterval(0); //not vsync


	curScene = std::make_unique<Scene>();
}

void Engine::update() {
	curScene->update();
}

void Engine::render() {
	curScene->render();
}
