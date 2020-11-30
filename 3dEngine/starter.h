#pragma once

#include <memory>

#include "display.h"
#include "engine.h"

class Starter {
private:
	std::unique_ptr<Display> display;
	std::unique_ptr<Engine> engine;

	void run(double delta);

public:
	void startup(double delta);
	
	void update();
	void render();
	
	void shutdown();

};

