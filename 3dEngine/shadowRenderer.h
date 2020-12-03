#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class ShadowRenderer {
private:
	const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	unsigned int depthMapFBO;
	unsigned int depthMap;

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;

public:
	ShadowRenderer();

	void render(Shader& shader, glm::vec3 lightPos);

	glm::mat4 getLightSpaceMatrix();
	int getDepthMap();
};

