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

	//model = std::make_unique<Model>("res/models/other/Small_Tropical_Island.obj");
	//model = std::make_unique<Model>("res/models/ruins/ruins.obj");
	model = std::make_unique<Model>("res/models/pier/woodenpier.obj");
	//model2 = std::make_unique<Model>("res/models/toher2/apple.obj");
	model2 = std::make_unique<Model>("res/models/cage/Cage.obj");
	//model = std::make_unique<Model>("res/models/backpack/backpack.obj");

	std::vector<std::string> faces {
        "res/textures/skybox/clouds1_east.bmp",
        "res/textures/skybox/clouds1_west.bmp",
        "res/textures/skybox/clouds1_up.bmp",
        "res/textures/skybox/clouds1_down.bmp",
        "res/textures/skybox/clouds1_north.bmp",
        "res/textures/skybox/clouds1_south.bmp"
    };
	skybox = std::make_unique<SkyBox>(faces);


	glGenFramebuffers(1, &depthMapFBO);  
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

Scene::Scene() {
	initRes();

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
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	ResourceManager::getShader("shadow").use();
	ResourceManager::getShader("shadow").setMat4("lightSpaceMatrix", lightSpaceMatrix);
	Shader s = ResourceManager::getShader("shadow");

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		renderScene(s);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//second pass
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ResourceManager::getShader("lighted").use();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	ResourceManager::getShader("lighted").setMat4("projection", projection);
	ResourceManager::getShader("lighted").setMat4("view", view);
	// set light uniforms
	ResourceManager::getShader("lighted").setVec3("viewPos", camera.getCameraPos());
	ResourceManager::getShader("lighted").setVec3("lightPos", lightPos);
	ResourceManager::getShader("lighted").setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	s = ResourceManager::getShader("lighted");
	renderScene(s);

	}

void Scene::renderScene(Shader& shader) {
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::scale(modelMat, glm::vec3(0.5, 0.5, 0.5));
	shader.use();
	shader.setMat4("model", modelMat);
	model->render(shader);

	modelMat = glm::translate(modelMat, glm::vec3(1, 2.45, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.02, 0.02, 0.02));
	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(-1, 0, 0));
	shader.setMat4("model", modelMat);
	model2->render(shader);

	ResourceManager::getShader("skybox").use();
	modelMat = glm::mat4(1.0f);
	ResourceManager::getShader("skybox").setMat4("model", modelMat);
	ResourceManager::getShader("skybox").setMat4("view", glm::mat4(glm::mat3(view)));
	Shader s = ResourceManager::getShader("skybox");
	skybox->render(s);

}
