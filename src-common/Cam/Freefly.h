#pragma once

#include "p6/p6.h"

namespace Camera {

class Freefly {
public:

    // Spherical coordinate
    glm::vec3 _position{};
    glm::vec3 _front;// front vector pointant en face de la caméra.
    glm::vec3 _left;
    glm::vec3 _up;
    
    float     _phi{p6::PI};
    float     _theta{};

    Freefly(glm::vec3 position = {}, float phi = 0.f, float theta = 0.f);
    glm::mat4 getViewMatrix() const;

    void moveLeft(float t);
    void moveFront(float t);
    void rotateLeft(float degrees);
    void rotateRight(float degrees);
    void rotateUp(float degrees);
    void rotateDown(float degrees);

    void handleEvent(p6::Context &ctx);
    void computeDirectionVectors();

};

} 
