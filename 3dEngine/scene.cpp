#include "scene.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resourceManager.h"
#include "inputManager.h"

Scene::Scene() {
	model = std::make_unique<Model>("res/models/other/Small_Tropical_Island.obj");
	model2 = std::make_unique<Model>("res/models/backpack/backpack.obj");

	std::vector<std::string> faces {
        "res/textures/skybox/clouds1_east.bmp",
        "res/textures/skybox/clouds1_west.bmp",
        "res/textures/skybox/clouds1_up.bmp",
        "res/textures/skybox/clouds1_down.bmp",
        "res/textures/skybox/clouds1_north.bmp",
        "res/textures/skybox/clouds1_south.bmp"
    };
	skybox = std::make_unique<SkyBox>(faces);

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

	ResourceManager::getShader("lighted").use();
	ResourceManager::getShader("lighted").setMat4("view", view);
	ResourceManager::getShader("lighted").setVec3("viewPos", cameraPos);


	ResourceManager::getShader("skybox").use();
	view = glm::mat4(glm::mat3(view)); 
	ResourceManager::getShader("skybox").setMat4("view", view);

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
	modelMat = glm::scale(modelMat, glm::vec3(0.03, 0.03, 0.03));
	modelMat = glm::translate(modelMat, glm::vec3(0, -75, 0));
	ResourceManager::getShader("lighted").use();
	ResourceManager::getShader("lighted").setMat4("model", modelMat);
	model->render(s);
	modelMat = glm::translate(modelMat, glm::vec3(0, 0, 800));
	modelMat = glm::translate(modelMat, glm::vec3(0, 0, -90));
	ResourceManager::getShader("lighted").setMat4("model", modelMat);
	model->render(s);

	modelMat = glm::mat4(1.0f);
	modelMat = glm::scale(modelMat, glm::vec3(0.2, 0.2, 0.2));
	ResourceManager::getShader("lighted").setMat4("model", modelMat);
	model2->render(s);

	ResourceManager::getShader("skybox").use();
	ResourceManager::getShader("skybox").setMat4("model", modelMat);
	s = ResourceManager::getShader("skybox");
	skybox->render(s);
}
