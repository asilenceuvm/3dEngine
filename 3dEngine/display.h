#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Display {
public:
	Display(int width, int height);
	~Display();
	
	GLFWwindow* getWindow();

private:
	GLFWwindow* window;
};
