#pragma once

#include <memory>

#include "model.h"
#include "camera.h"
#include "skybox.h"
#include "shadowRenderer.h"
#include "gameObject.h"

class Scene {
private:
	std::unique_ptr<SkyBox> skybox;
	std::unique_ptr<ShadowRenderer> shadowRenderer;
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

	static glm::vec3 lightPos;
	static std::map<std::string, std::shared_ptr<Model>> models;
	static std::map<std::string, GameObject> gameObjects;
};

