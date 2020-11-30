#pragma once
class Engine {
private:
	int width, height;
public:
	Engine(int width, int height);
	void update();
	void render();
};

