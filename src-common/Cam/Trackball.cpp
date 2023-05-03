#include "Trackball.h"

#include <glm/gtc/type_ptr.hpp>

namespace Camera {

void Trackball::moveFront(float delta) {
    _distance += delta;
}
void Trackball::rotateLeft(float degrees) {
    _angleX += degrees;
}
void Trackball::rotateUp(float degrees) {
    _angleY += degrees;
}

glm::mat4 Trackball::getViewMatrix() const {
    float const angleX = _angleX * (p6::PI/180);
    float const angleY = _angleY * (p6::PI/180);

    auto view = glm::mat4{1};
    view = glm::translate(view, glm::vec3{0.f, 0.f, _distance});
    view = glm::rotate(view, angleX, glm::vec3{0.f, 1.f, 0.f}); // car on tourne sur Y
    view = glm::rotate(view, angleY, glm::vec3{-1.f, 0.f, 0.f}); // car on va vers le haut
    return view;
}



}