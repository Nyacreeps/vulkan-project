#include "camera.hh"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

glm::vec3 Camera::getFront(void) {
    return center - eye;
}
void Camera::move(CAM_MOVE_DIR dir, float deltaTime, float speed /* = 1.0f */) {
    float distance = deltaTime * speed;
    glm::vec3 moveVector;
    switch (dir) {
        case CAM_MOVE_FRONT:
            moveVector = getFront();
            break;
        case CAM_MOVE_BACK:
            moveVector = -getFront();
            break;
        case CAM_MOVE_LEFT:
            moveVector = glm::cross(axis, getFront());
            break;
        case CAM_MOVE_RIGHT:
            moveVector = glm::cross(getFront(), axis);
            break;
        case CAM_MOVE_UP:
            moveVector = axis;
            break;
        case CAM_MOVE_DOWN:
            moveVector = -axis;
            break;
        default:
            moveVector = getFront();
            break;
    }
    center += moveVector * distance;
    eye += moveVector * distance;
}

static glm::mat4 getRotMatrix(const glm::vec3& axis, float angle, const glm::vec3& origin) {
    auto translateMatrix = glm::translate(glm::mat4(1.0f), -origin);
    auto translateMatrixBack = glm::translate(glm::mat4(1.0f), origin);
    auto rotMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
    return translateMatrixBack * rotMatrix * translateMatrix;
}

void Camera::roll(double xAxisMove, double yAxisMove, float speed /* = 1.0f */) {
    if (!(xAxisMove || yAxisMove))
        return;

    center = getRotMatrix(axis, xAxisMove * speed / 100.0f, eye) * glm::vec4(center, 1.0f);
    up = glm::normalize(glm::vec3(getRotMatrix(axis, xAxisMove * speed / 100.0f, {0.0f, 0.0f, 0.0f}) * glm::vec4(up, 1.0f)));

    glm::vec3 left = glm::normalize(glm::cross(up, getFront()));
    
    center = getRotMatrix(left, yAxisMove * speed / 100.0f, eye) * glm::vec4(center, 1.0f);
    up = glm::normalize(glm::vec3(getRotMatrix(left, yAxisMove * speed / 100.0f, {0.0f, 0.0f, 0.0f}) * glm::vec4(up, 1.0f)));
}

glm::mat4 Camera::getViewMatrix(void) {
    return glm::lookAt(eye, center, up);
}

glm::mat4 Camera::getProjectionMatrix(const float& aspectRatio) {
    auto proj = glm::perspective(fov, aspectRatio, nearClipDist, farClipDist);
    proj[1][1] *= -1;
    return proj;
}