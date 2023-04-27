#include "Freefly.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Camera {

Freefly::Freefly(glm::vec3 position, float phi, float theta)
    : _position(position), _phi(phi), _theta(theta)
{
    computeDirectionVectors();
}

void Freefly::computeDirectionVectors()
{
    _front = glm::vec3{
        cos(_theta) * sin(_phi),
        sin(_theta),
        cos(_theta) * cos(_phi)};
    _left = glm::vec3{
        sin(_phi + p6::PI / 2),
        0,
        cos(_phi + p6::PI / 2)};
    _up = glm::cross(_front, _left);
}

void Freefly::moveLeft(float t)
{
    _position -= t * _left;
}

void Freefly::moveFront(float t)
{
    _position += t * _front;
}

// ToDo: Up And Down

void Freefly::rotateLeft(float degrees)
{
    const auto radians = degrees * p6::PI / 180;
    _phi -= radians;
    computeDirectionVectors();
}

void Freefly::rotateUp(float degrees)
{
    const auto radians = degrees * p6::PI / 180;
    _theta += radians;
    computeDirectionVectors();
}

glm::mat4 Freefly::getViewMatrix() const
{
    return glm::lookAt(_position, _position + _front, _up);
}

} // namespace Camera