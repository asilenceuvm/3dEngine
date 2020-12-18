#include "engine.h"

#include "python38/Python.h"

#include "resourceManager.h"
#include "starter.h"
#include <thread>


void runPython() {
	std::cout << "Python Terminal: " << std::endl;
	bool shouldclose = false;
	bool reload = false;
	//while (shouldclose == false) {
		//PyImport_AppendInittab("arnav", &PyInit_arnav);

		Py_Initialize();
		//std::string input;
		//while (true) {
		//	std::cout << ">> ";
		//	std::cin >> input;
		//	std::cout << input << std::endl;
		//	PyRun_SimpleString(input.c_str());
		//}
		std::string s;
		std::cout << ">> ";
		while (std::getline(std::cin, s) && (reload == false)) {
			PyRun_SimpleString(s.c_str());
			std::cout << ">> ";
		}
		Py_Finalize();
	//}
}

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
	glfwWindowHint(GLFW_SAMPLES, 4); //4x MSAA, this is for polygons, not textures
	glEnable(GL_MULTISAMPLE);
	
	//vsync
	//glfwSwapInterval(1); //vsync
	glfwSwapInterval(0); //not vsync

	loadRes();

	curScene = std::make_unique<Scene>(width, height);

	std::thread (runPython).detach();
}

void Engine::update() {
	curScene->update();
}

void Engine::render() {
	curScene->render();
}

void Engine::loadRes() {
	ResourceManager::loadShader("res/shaders/lighted.vert", "res/shaders/lighted.frag", nullptr, "lighted");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(width / height), 0.1f, 100.0f);
	
	ResourceManager::getShader("lighted").use().setInt("image", 0);
	ResourceManager::getShader("lighted").setMat4("projection", projection);
	ResourceManager::getShader("lighted").setInt("diffuseTexture", 0);
    ResourceManager::getShader("lighted").setInt("shadowMap", 1);


	ResourceManager::loadShader("res/shaders/skybox.vert", "res/shaders/skybox.frag", nullptr, "skybox");
	ResourceManager::getShader("skybox").use().setInt("skybox", 0);
	ResourceManager::getShader("skybox").setMat4("projection", projection);

	ResourceManager::loadShader("res/shaders/shadow.vert", "res/shaders/shadow.frag", nullptr, "shadow");
}
