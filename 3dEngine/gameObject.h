#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "model.h"

class GameObject {
private:
	std::shared_ptr<Model> model;
	glm::vec3 pos = glm::vec3(0);
	glm::vec3 rotate = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

public:
	GameObject(std::shared_ptr<Model> model);

	void render(Shader& shader);

	void setPos(glm::vec3 pos);
	void setRotate(glm::vec3 rotate);
	void setScale(glm::vec3 scale);
};

