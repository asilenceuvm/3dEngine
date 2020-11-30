#pragma once

#include <memory>

#include "scene.h"

class Engine {
private:
	int width = 0, height = 0;
	std::unique_ptr<Scene> curScene;

public:
	Engine(int width, int height);
	void update();
	void render();
};

