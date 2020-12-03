#pragma once

#include <memory>

#include "model.h"
#include "camera.h"
#include "skybox.h"
#include "shadowRenderer.h"

class Scene {
private:
	//temp
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> model2;

	std::unique_ptr<SkyBox> skybox;
	std::unique_ptr<ShadowRenderer> shadowRenderer;
	glm::vec3 lightPos;
	glm::mat4 view;

	Camera camera;

	int width, height;

	//load values specific to the scene
	void initRes();

	//helper method to draw scene
	void renderScene(Shader& shader);
public:
	Scene(int width, int height);
	void update();
	void render();
};

