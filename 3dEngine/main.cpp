#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "logger.h"
#include "starter.h"

int main() {
	Logger::logInfo("ENGINE", "Starting engine");

	Starter s;
	s.startup(1.0 / 120.0);

	return 0;
}