#include "gameObject.h"

GameObject::GameObject(std::shared_ptr<Model> model) {
	this->model = model;
}

void GameObject::render(Shader& shader) {
	model->render(shader, pos, scale, rotate);
}

void GameObject::setPos(glm::vec3 pos) {
	this->pos = pos;
}

void GameObject::setRotate(glm::vec3 rotate) {
	this->rotate = rotate;
}

void GameObject::setScale(glm::vec3 scale) {
	this->scale = scale;
}
