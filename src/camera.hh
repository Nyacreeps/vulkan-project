#pragma once

#include <glm/glm.hpp>
#include <iostream>

typedef enum CAM_MOVE_DIR {
    CAM_MOVE_FRONT,
    CAM_MOVE_BACK,
    CAM_MOVE_LEFT,
    CAM_MOVE_RIGHT,
    CAM_MOVE_UP,
    CAM_MOVE_DOWN,
    MOVE_DIR_MAX_ENUM = 0x7FFFFFFF
} CAM_MOVE_DIR;

class Camera {
private:
    glm::vec3 eye;
    glm::vec3 center;
    const glm::vec3 axis;
    glm::vec3 up;
    float fov;
    float nearClipDist;
    float farClipDist;
    glm::vec3 getFront(void);

public:
    Camera(const glm::vec3& camEye, const glm::vec3& camCenter, const glm::vec3& camAxis,
           float camFov, float camNearClipDist, float camFarClipDist)
        : eye(camEye),
          center(camCenter),
          axis(glm::normalize(camAxis)),
          up(glm::normalize(camAxis)),
          fov(camFov),
          nearClipDist(camNearClipDist),
          farClipDist(camFarClipDist){};
    void move(CAM_MOVE_DIR dir, float deltaTime, float speed = 1.0f);
    void roll(double xAxisMove, double yAxisMove, float speed = 1.0f);
    glm::mat4 getViewMatrix(void);
    glm::mat4 getProjectionMatrix(const float& aspectRatio);
};