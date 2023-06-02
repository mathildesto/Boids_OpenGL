#include "p6/p6.h"
#include "3DBoidsMouvement.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include<cmath>
#include "glimac/sphere_vertices.hpp"

float compute_distance(const Boid3D boid1, const Boid3D boid2 ){
        //distance = sqrt((x2 - x1)² + (y2 - y1)²)
        return sqrt(pow(boid2.position[0] - boid1.position[0],2) + pow(boid2.position[1] - boid1.position[1],2) + pow(boid2.position[2] - boid1.position[2],2));
    };

void separation(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid){
    float close_dx= 0;
    float close_dy= 0;
    float close_dz= 0;

    for(int j = 0; j<boids.size() ; j++){

        if (boid == boids[j] )
            continue;

        float distance = compute_distance(boid, boids[j]);
    
        //Is squared distance less than the protected range?
        if (distance < param.protectedRange){

            //If so, calculate difference in x/y-coordinates to nearfield boid
            close_dx += boid.position[0] - boids[j].position[0]; 
            close_dy += boid.position[1] - boids[j].position[1];
            close_dz += boid.position[2] - boids[j].position[2];
        }
    }

    boid.velocity[0] += (close_dx*param.avoidfactor); // Add the avoidance contribution to velocity
    boid.velocity[1] += (close_dy*param.avoidfactor);
    boid.velocity[2] += (close_dz*param.avoidfactor);

}

void cohesion(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid){

    float xvel_avg= 0;
    float yvel_avg= 0;
    float zvel_avg= 0;
    float neighboring_boids= 0;

    for(int j = 0; j<boids.size() ; j++){

        if (boid == boids[j] )
            continue;

        float distance = compute_distance(boid, boids[j]);

        //Is squared distance less than the visual range?
        if (distance < param.visualRange){

            //Add other boid's x/y-coord and x/y vel to accumulator variables
            xvel_avg += boids[j].velocity[0];
            yvel_avg += boids[j].velocity[1];
            zvel_avg += boids[j].velocity[2];

            //Increment number of boids within visual range
            neighboring_boids += 1; 
        }
    }

    //If there were any boids in the visual range . . .            
    if (neighboring_boids > 0){ 

        //Divide accumulator variables by number of boids in visual range
        xvel_avg = xvel_avg/neighboring_boids;
        yvel_avg = yvel_avg/neighboring_boids;
        zvel_avg = zvel_avg/neighboring_boids;

        boid.velocity[0] = boid.velocity[0] + (xvel_avg - boid.velocity[0])*param.matchingfactor; //Add the centering/matching contributions to velocity
        boid.velocity[1] = boid.velocity[1]  + (yvel_avg - boid.velocity[1])*param.matchingfactor;
        boid.velocity[2] = boid.velocity[2]  + (zvel_avg - boid.velocity[2])*param.matchingfactor;
    }
}

void keep_inside_boundaries(ParamBoids3D &param, Window3D &window, Boid3D &boid){
    if (boid.position[0] < 0.8*window.WINDOW_MIN_X) {
        boid.velocity[0] += param.turnfactor ; // reverse the x-component of the velocity
    }
    else if (boid.position[0] > 0.8*window.WINDOW_MAX_X) {
        boid.velocity[0] += -param.turnfactor;
    }
    if (boid.position[1] < 0.8*window.WINDOW_MIN_Y) {
        boid.velocity[1] += param.turnfactor;
    }
    else if (boid.position[1] > 0.8*window.WINDOW_MAX_Y) {
        boid.velocity[1] += -param.turnfactor;
    }

    if (boid.position[2] < 0.8*window.WINDOW_MIN_Z) {
        boid.velocity[2] += param.turnfactor;
    }
    else if (boid.position[2] > 0.8*window.WINDOW_MAX_Z) {
        boid.velocity[2] += -param.turnfactor;
    }
}

void limit_speed(ParamBoids3D &param, Boid3D &boid){

    float speed = sqrt(boid.velocity[0]*boid.velocity[0] + boid.velocity[1]*boid.velocity[1] + boid.velocity[2]*boid.velocity[2]);

    if (speed < param.minspeed){
        boid.velocity[0] = (boid.velocity[0]/speed)*param.minspeed;
        boid.velocity[1] = (boid.velocity[1]/speed)*param.minspeed;
        boid.velocity[2] = (boid.velocity[2]/speed)*param.minspeed;
    }
    if (speed > param.maxspeed){
        boid.velocity[0] = (boid.velocity[0]/speed)*param.maxspeed;
        boid.velocity[1] = (boid.velocity[1]/speed)*param.maxspeed;
        boid.velocity[2] = (boid.velocity[2]/speed)*param.maxspeed;
    }
}


void alignment(std::vector<Boid3D>& boids, ParamBoids3D &param, Boid3D &boid){

    float xpos_avg = 0;
    float ypos_avg= 0;
    float zpos_avg= 0;
    float neighboring_boids= 0;

    for(int j = 0; j < boids.size() ; j++){

        if (boid == boids[j] )
            continue;

        float distance = compute_distance(boid, boids[j]);
    
        //Is squared distance less than the protected range?
        if (distance < param.visualRange){

            //Add other boid's x/y-coord and x/y vel to accumulator variables
            xpos_avg += boids[j].position[0] ;
            ypos_avg += boids[j].position[1] ;
            zpos_avg += boids[j].position[2] ;

        //Increment number of boids within visual range
            neighboring_boids += 1; 
        }
    }

    //If there were any boids in the visual range . . .            
    if (neighboring_boids > 0){ 

        //Divide accumulator variables by number of boids in visual range
        xpos_avg = xpos_avg/neighboring_boids; 
        ypos_avg = ypos_avg/neighboring_boids;
        zpos_avg = zpos_avg/neighboring_boids;

        boid.velocity[0] += (xpos_avg - boid.position[0])*param.centeringfactor; //Add the centering/matching contributions to velocity
        boid.velocity[1] += (ypos_avg - boid.position[1])*param.centeringfactor;
        boid.velocity[2] += (zpos_avg - boid.position[2])*param.centeringfactor;
    }
}


std::vector<Boid3D> initialise_positions(int n){
    // Adds all the boids at a starting position. I put them all at random locations

    std::vector<Boid3D> boids(n);
    for (auto& boid : boids){
        boid = Boid3D();
    }
    return boids;
}

void update_position(std::vector<Boid3D>& boids, Window3D &window, ParamBoids3D &param, p6::Context&ctx){
    
    // for each boid
    for (auto& boid : boids){

        alignment(boids, param, boid);
        
        cohesion (boids, param, boid);

        separation(boids, param, boid);

        keep_inside_boundaries(param, window, boid);

        limit_speed(param, boid);

        boid.add_velocity();
                
        // Update boid's orientation
        glm::vec3 direction = glm::normalize(boid.velocity);
        // Calculate the angles for rotation
        float yaw = glm::degrees(atan2(direction.x, direction.z));
        float pitch = glm::degrees(asin(-direction.y));

        // Set the boid's orientation
        // Assuming you have a `glm::vec3` member variable named `orientation` in the `Boid3D` struct
        boid.orientation = glm::vec3(pitch, yaw, 0.0);

    }
}


