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
	Camera camera;
public:
	Scene();
	void update();
	void render();
};

