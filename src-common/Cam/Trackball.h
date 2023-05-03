#pragma once

#include <p6/p6.h>

namespace Camera {

class Trackball {
public:
    Trackball(float distance = -5, float angleX = 0, float angleY = 0)
        : _distance(distance), _angleX(angleX), _angleY(angleY) {}

    void moveFront(float delta);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;

private:
    float _distance;
    float _angleX;
    float _angleY;
};

} // namespace Camera
