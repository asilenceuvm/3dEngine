#include "scene.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resourceManager.h"
#include "inputManager.h"

Scene::Scene() {
	model = std::make_unique<Model>("res/models/backpack/backpack.obj");


	
	ResourceManager::loadShader("res/shaders/lighted.vert", "res/shaders/lighted.frag", nullptr, "lighted");

	//TODO: change to width and height
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(800 / 600), 0.1f, 100.0f);
	
	ResourceManager::getShader("lighted").use().setInt("image", 0);
	ResourceManager::getShader("lighted").setMat4("projection", projection);

	ResourceManager::getShader("lighted").setInt("material.diffuse", 0);
	ResourceManager::getShader("lighted").setInt("material.specular", 1);
	ResourceManager::getShader("lighted").setFloat("material.shine", 64.0f);
	
	ResourceManager::getShader("lighted").setVec3("dirLight.direction", 0.0f, -0.0f, -1.0f);
	ResourceManager::getShader("lighted").setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	ResourceManager::getShader("lighted").setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	ResourceManager::getShader("lighted").setVec3("dirLight.specular", 0.3f, 0.3f, 0.3f);

	ResourceManager::getShader("lighted").setVec3("pointLight.position", 0.0f, 1.0f, -2.0f);
	ResourceManager::getShader("lighted").setFloat("pointLight.constant", 1.0f);
	ResourceManager::getShader("lighted").setFloat("pointLight.linear", 0.09f);
	ResourceManager::getShader("lighted").setFloat("pointLight.quadratic", 0.032f);
	ResourceManager::getShader("lighted").setVec3("pointLight.ambient", 0.2f, 0.2f, 0.2f);
	ResourceManager::getShader("lighted").setVec3("pointLight.diffuse", 0.5f, 0.5f, 0.5f);
	ResourceManager::getShader("lighted").setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);

	ResourceManager::getShader("lighted").setVec3("objectColor", 0.8f, 0.8f, 0.8f);
	ResourceManager::getShader("lighted").setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	ResourceManager::getShader("lighted").setVec3("lightPos", 0.0f, 1.0f, -3.0f);
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
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	ResourceManager::getShader("lighted").setMat4("view", view);
	ResourceManager::getShader("lighted").setVec3("viewPos", cameraPos);
}

void Scene::render() {
	Shader s = ResourceManager::getShader("lighted");
	glm::mat4 modelMat = glm::mat4(1.0f);
	ResourceManager::getShader("lighted").setMat4("model", modelMat);
	model->render(s);
}
