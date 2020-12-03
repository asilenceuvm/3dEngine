#include "scene.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resourceManager.h"
#include "inputManager.h"

void Scene::initRes() {
	lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
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

	model = std::make_unique<Model>("res/models/pier/woodenpier.obj");
	model2 = std::make_unique<Model>("res/models/cage/Cage.obj");

	std::vector<std::string> faces {
        "res/textures/skybox/clouds1_east.bmp",
        "res/textures/skybox/clouds1_west.bmp",
        "res/textures/skybox/clouds1_up.bmp",
        "res/textures/skybox/clouds1_down.bmp",
        "res/textures/skybox/clouds1_north.bmp",
        "res/textures/skybox/clouds1_south.bmp"
    };
	skybox = std::make_unique<SkyBox>(faces);

	shadowRenderer = std::make_unique<ShadowRenderer>();
}

Scene::Scene(int width, int height) {
	initRes();

	this->width = width;
	this->height = height;

	//starting values for camera
	InputManager::xoffset = 0;
	InputManager::yoffset = 0;
	camera.rotateCamera(-90, 0, 1);
}

void Scene::update() {
	//temp
	if (InputManager::keys[GLFW_KEY_W]) {
		camera.moveCamForward(0.05f);
	}
	if (InputManager::keys[GLFW_KEY_S]) {
		camera.moveCamBack(0.05f);
	}
	if (InputManager::keys[GLFW_KEY_A]) {
		camera.moveCamLeft(0.05f);
	}
	if (InputManager::keys[GLFW_KEY_D]) {
		camera.moveCamRight(0.05f);
	}
	camera.rotateCamera(InputManager::xoffset, InputManager::yoffset, 0.1f);
	InputManager::xoffset = 0;
	InputManager::yoffset = 0;
	glm::vec3 cameraPos = camera.getCameraPos();
	glm::vec3 cameraFront = camera.getCameraFront();
	glm::vec3 cameraUp = camera.getCameraUp();
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//temp
	if (InputManager::keys[GLFW_KEY_F]) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	}

	if (InputManager::keys[GLFW_KEY_V]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}


}

void Scene::render() {
	//first pass
	lightPos = glm::vec3(sin(glfwGetTime()), 3, 2);

	Shader s = ResourceManager::getShader("shadow");
	shadowRenderer->render(s, lightPos);
	renderScene(s);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//second pass
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ResourceManager::getShader("lighted").use();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	ResourceManager::getShader("lighted").setMat4("projection", projection);
	ResourceManager::getShader("lighted").setMat4("view", view);
	// set light uniforms
	ResourceManager::getShader("lighted").setVec3("viewPos", camera.getCameraPos());
	ResourceManager::getShader("lighted").setVec3("lightPos", lightPos);
	ResourceManager::getShader("lighted").setMat4("lightSpaceMatrix", shadowRenderer->getLightSpaceMatrix());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowRenderer->getDepthMap());

	s = ResourceManager::getShader("lighted");
	renderScene(s);

	//draw skybox
	s = ResourceManager::getShader("skybox");
	skybox->render(s, view);
}

void Scene::renderScene(Shader& shader) {
	model->render(shader, glm::vec3(0,0,0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 0, 0));
	model2->render(shader, glm::vec3(0.5, 1.2, 0), glm::vec3(0.01, 0.01, 0.01), glm::vec3(-1, 0, 0));
}
