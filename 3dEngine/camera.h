#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class Camera {
private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    glm::vec3 direction = glm::vec3(1);
    float yaw = 0, pitch = 0;
public:
    Camera();

    void moveCamForward(const float delta);
    void moveCamBack(const float delta);
    void moveCamLeft(const float delta);
    void moveCamRight(const float delta);

    void rotateCamera(float xoffset, float yoffset, float sensitivity);

    glm::vec3 getCameraPos();
    glm::vec3 getCameraFront();
    glm::vec3 getCameraUp();
};