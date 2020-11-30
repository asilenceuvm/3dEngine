#pragma once

#include <memory>

#include "model.h"
#include "camera.h"

class Scene {
private:
	//temp
	std::unique_ptr<Model> model;
	Camera camera;
public:
	Scene();
	void update();
	void render();
};

