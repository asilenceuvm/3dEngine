#include "scene.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resourceManager.h"
#include "inputManager.h"

Scene::Scene() {
	model = std::make_unique<Model>("res/models/backpack/backpack.obj");

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
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	ResourceManager::getShader("lighted").setMat4("view", view);
	ResourceManager::getShader("lighted").setVec3("viewPos", cameraPos);

	//temp
	if (InputManager::keys[GLFW_KEY_F]) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	}

	if (InputManager::keys[GLFW_KEY_V]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Scene::render() {
	Shader s = ResourceManager::getShader("lighted");
	glm::mat4 modelMat = glm::mat4(1.0f);
	ResourceManager::getShader("lighted").setMat4("model", modelMat);
	model->render(s);
}
