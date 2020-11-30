#include "starter.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "logger.h"
#include "inputManager.h"

//set up image processor
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam ) {
	const GLchar* log = (stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
	if (type == GL_DEBUG_TYPE_ERROR) {
		Logger::logError("OPENGL", log);
	}
	else {
		Logger::logWarning("OPENGL", log, false);
	}
}

void Starter::startup(double delta) {
	int width = 800;
	int height = 600;
	display = std::make_unique<Display>(width, height);
	engine = std::make_unique<Engine>(width, height);

	//set up input
	InputManager::lastX = static_cast<float>(width / 2);
	InputManager::lastY = static_cast<float>(height / 2);
	glfwSetKeyCallback(display->getWindow(), InputManager::key_callback);
	glfwSetInputMode(display->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(display->getWindow(), InputManager::mouse_callback);

	//set up debug messages
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	run(delta);
}

void Starter::shutdown() {
	glfwSetWindowShouldClose(display->getWindow(), GLFW_TRUE);
}

void Starter::update() {
	if (InputManager::keys[GLFW_KEY_ESCAPE]) {
		shutdown();
	}
	engine->update();
}

void Starter::render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	engine->render();

	glfwSwapBuffers(display->getWindow());
	glfwPollEvents();
}

void Starter::run(double delta) {
	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	//enter loop
	while (!glfwWindowShouldClose(display->getWindow())) {

		//get time
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / delta;
		lastTime = nowTime;

		//update at delta
		while (deltaTime >= 1.0) {
			update();
			updates++;
			deltaTime--;
		}

		render();
		frames++;

		//reset and output fps
		if (glfwGetTime() - timer > 1.0) {
			timer++;
			//std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
			updates = 0, frames = 0;
		}
	}
}
