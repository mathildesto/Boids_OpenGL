#pragma once
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <iostream>
#include<cmath>


struct Boid3D {
    float size;
    glm::vec3 position;
    glm::vec3 velocity;

    Boid3D() {
        position = glm::vec3(p6::random::number(-1, 1), p6::random::number(-1, 1), p6::random::number(-1, 1));
        velocity = glm::vec3(0, 0, 0);
    }

    Boid3D(glm::vec3 pos, glm::vec3 vel) {
        position = pos;
        velocity = vel;
    }

    bool operator ==(const Boid3D& boid) const {
        return position == boid.position && velocity == boid.velocity;
    }

    void add_velocity() {
        position += velocity;
    }
};


struct ParamBoids3D{
    int numberOfBoids = 50;

    float boidSize = 0.1f;

    float visualRange = 0.8 ; 
    float protectedRange = 0.5 ; 

    float turnfactor = 0.005 ; 
    float centeringfactor = 0.0005;
    float avoidfactor =  0.04;
    float matchingfactor = 0.05;
    
    float maxspeed = 0.04;
    float minspeed = 0.009;
};

struct Window3D {

    float WINDOW_MIN_X = -3.;
    float WINDOW_MAX_X = 3;

    float WINDOW_MIN_Y = -2.;
    float WINDOW_MAX_Y = 2.;

    float WINDOW_MIN_Z = -2.;
    float WINDOW_MAX_Z = 2.;
};

float compute_distance(const Boid3D boid1, const Boid3D boid2 );

std::vector<Boid3D> initialise_positions(int n);


void keep_inside_boundaries(ParamBoids3D &param, Window3D &window, Boid3D &boid);
void limit_speed(ParamBoids3D &param, Boid3D &boid);
void separation(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid);
void cohesion(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid);
void alignment(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid);

void update_position(std::vector<Boid3D>& boids, Window3D& window, ParamBoids3D &param);

/**
void draw_boids(Camera::Freefly freefly, p6::Context& ctx, GLuint vao, std::vector<Boid3D> boids, ParamBoids3D param, const GLint uMVPMatrix, const GLint uMVMatrix, const GLint uNormalMatrix, const GLint uKdVector, const GLint uKsVector, const GLint uShininessFloat, const GLint uLightDirVector, const GLint uLightIntensityVector 
   ){
    auto modelViewMatrix  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
    auto projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); 
    auto normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);

    glUniform3fv(uKdVector, 1, glm::value_ptr(glm::vec3{0.5, 0.5, 0.5}));
    glUniform3fv(uKsVector, 1, glm::value_ptr(glm::vec3{0.5, 0.5, 0.5}));
    glUniform1f(uShininessFloat, 1.f);
    glUniform3fv(uLightDirVector, 1, glm::value_ptr(glm::vec3(glm::mat4{1} * glm::vec4{1.f,1.f,1.f, 1.f}))); 
    //glUniform3fv(uLightDirVector, 1, glm::value_ptr(glm::vec3(freefly.getViewMatrix() * glm::vec4{1.f,1.f,1.f, 1.f}))); // mat4 -> vec3 ???

    glUniform3fv(uLightIntensityVector, 1, glm::value_ptr(glm::vec3{1.f, 1.f, 1.f}));


    for (size_t i = 0; i < boids.size(); i++)
    {
        modelViewMatrix = glm::translate(glm::mat4{1}, glm::vec3(0.f, 0.f, -5.f));
        //modelViewMatrix = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        modelViewMatrix = glm::translate(modelViewMatrix, boids[i].position);
        modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{param.boidSize});
        normalMatrix    = glm::transpose(glm::inverse(modelViewMatrix));

        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
}
**/




