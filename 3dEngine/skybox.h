#pragma once

#include <string>
#include <vector>

#include "shader.h"

class SkyBox {
private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;

	// loads a cubemap texture from 6 textures 
	// order: +x -x +y -y +z -z (right, left, top, bottom, front, back)
	unsigned int loadCubemap(std::vector<std::string> faces);

public:
	SkyBox(std::vector<std::string> faces);

	void render(Shader& shader);
};

