#include "display.h"

#include "Logger.h"

Display::Display(int width, int height) {
	Logger::logMessage("OPENGL", "Creating window");
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	if (window == NULL) {
		Logger::logError("OPENGL", "Creating window");
		glfwTerminate();
	}
	else {
		Logger::logSuccess("OPENGL", "Creating window");
	}

	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);
}

Display::~Display() {

}

GLFWwindow* Display::getWindow() {
	return this->window;
}
