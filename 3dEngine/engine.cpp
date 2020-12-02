#include "engine.h"

#include "resourceManager.h"

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

	curScene = std::make_unique<Scene>();
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

	ResourceManager::getShader("lighted").setInt("material.diffuse", 0);
	ResourceManager::getShader("lighted").setInt("material.specular", 1);
	ResourceManager::getShader("lighted").setFloat("material.shine", 256.0f);
	
	ResourceManager::getShader("lighted").setVec3("dirLight.direction", 0.0f, -0.0f, -1.0f);
	ResourceManager::getShader("lighted").setVec3("dirLight.ambient", 0.7f, 0.7f, 0.7f);
	ResourceManager::getShader("lighted").setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	ResourceManager::getShader("lighted").setVec3("dirLight.specular", 0.3f, 0.3f, 0.3f);

	ResourceManager::getShader("lighted").setVec3("pointLight.position", 0.0f, 1.0f, -200.0f);
	ResourceManager::getShader("lighted").setFloat("pointLight.constant", 1.0f);
	ResourceManager::getShader("lighted").setFloat("pointLight.linear", 0.09f);
	ResourceManager::getShader("lighted").setFloat("pointLight.quadratic", 0.032f);
	ResourceManager::getShader("lighted").setVec3("pointLight.ambient", 0.2f, 0.2f, 0.2f);
	ResourceManager::getShader("lighted").setVec3("pointLight.diffuse", 0.5f, 0.5f, 0.5f);
	ResourceManager::getShader("lighted").setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);

	ResourceManager::getShader("lighted").setVec3("objectColor", 0.8f, 0.8f, 0.8f);
	ResourceManager::getShader("lighted").setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	ResourceManager::getShader("lighted").setVec3("lightPos", 0.0f, 1.0f, -3.0f);


	ResourceManager::loadShader("res/shaders/skybox.vert", "res/shaders/skybox.frag", nullptr, "skybox");
	ResourceManager::getShader("skybox").use().setInt("skybox", 0);
	ResourceManager::getShader("skybox").setMat4("projection", projection);

}
