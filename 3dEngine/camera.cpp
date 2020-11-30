#include "camera.h"


Camera::Camera() {
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::moveCamForward(const float delta) {
	cameraPos += delta * cameraFront;
}
void Camera::moveCamBack(const float delta) {
	cameraPos -= delta * cameraFront;
}
void Camera::moveCamLeft(const float delta) {
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * delta;
}
void Camera::moveCamRight(const float delta) {
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * delta;
}

void Camera::rotateCamera(float xoffset, float yoffset, float sensitivity) {
	yaw += xoffset * sensitivity;
	pitch += yoffset * sensitivity;

	//clamp y look
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

glm::vec3 Camera::getCameraPos() {
	return cameraPos;
}

glm::vec3 Camera::getCameraFront() {
	return cameraFront;
}

glm::vec3 Camera::getCameraUp() {
	return cameraUp;
}
