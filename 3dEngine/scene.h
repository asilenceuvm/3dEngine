#pragma once

#include <memory>

#include "model.h"
#include "camera.h"
#include "skybox.h"

class Scene {
private:
	//temp
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> model2;
	std::unique_ptr<SkyBox> skybox;
	glm::vec3 lightPos;
	glm::mat4 view;
	const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	unsigned int depthMapFBO;
	unsigned int depthMap;

	Camera camera;

	//load values specific to the scene
	void initRes();

	//helper method to draw scene
	void renderScene(Shader& shader);
public:
	Scene();
	void update();
	void render();
};

