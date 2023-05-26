#pragma once
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <iostream>
#include<cmath>
#include "glimac/sphere_vertices.hpp"

struct ParamBoids3D{
    int numberOfBoids = 50;

    float boidSize = 0.005f;

    float visualRange = 0.3 ; 
    float protectedRange = 0.08 ; 

    float turnfactor = 0.015 ; 
    float centeringfactor = 0.0005;
    float avoidfactor =  0.03;
    float matchingfactor = 0.03;
    
    float maxspeed = 0.005;
    float minspeed = 0.0001;
};

struct Window3D {
    
    float WINDOW_MIN_X = -0.5;
    float WINDOW_MAX_X = 0.5;

    float WINDOW_MIN_Y = -0.5;
    float WINDOW_MAX_Y = 0.5;

    float WINDOW_MIN_Z = -0.5;
    float WINDOW_MAX_Z = 0.5;
};

struct Boid3D {
    float size;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 orientation; 
    glm::vec3 targetOrientation;


    Boid3D() {
        position = glm::vec3(p6::random::number(-0.5, 0.5), p6::random::number(-0.5, 0.5), p6::random::number(-0.5, 0.5));
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


float compute_distance(const Boid3D boid1, const Boid3D boid2 );

std::vector<Boid3D> initialise_positions(int n);


void keep_inside_boundaries(ParamBoids3D &param, Window3D &window, Boid3D &boid);
void limit_speed(ParamBoids3D &param, Boid3D &boid);
void separation(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid);
void cohesion(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid);
void alignment(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid);

void update_position(std::vector<Boid3D>& boids, Window3D& window, ParamBoids3D &param);





