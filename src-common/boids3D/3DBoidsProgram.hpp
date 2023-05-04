#pragma once
#include "Cam/Trackball.h"
#include "boids3D/boid3D.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <iostream>
#include "Cam/Freefly.h"
#include "Cam/Trackball.h"
#include<cmath>
#include "glimac/sphere_vertices.hpp"

struct BoidsProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;
    GLint uKdVector;
    GLint uKsVector;
    GLint uShininessFloat; 
    GLint uLightDirVector;   
    GLint uLightIntensityVector;

    GLuint vbo, vao;

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);


    BoidsProgram() : m_Program(p6::load_shader("shaders/3D_light.vs.glsl", "shaders/directionalLight.fs.glsl"))
    {
        uMVPMatrix              = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix               = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix           = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uEarthTexture           = glGetUniformLocation(m_Program.id(), "uEarthTexture");
        uCloudTexture           = glGetUniformLocation(m_Program.id(), "uCloudTexture");
        uKdVector               = glGetUniformLocation(m_Program.id(), "uKd");
        uKsVector               = glGetUniformLocation(m_Program.id(), "uKs");
        uShininessFloat         = glGetUniformLocation(m_Program.id(), "uShininess");
        uLightDirVector         = glGetUniformLocation(m_Program.id(), "uLightDir_vs");
        uLightIntensityVector   = glGetUniformLocation(m_Program.id(), "uLightIntensity");

    }

    void resize(ParamBoids3D& param, std::vector<Boid3D> & boids){
    int dif = static_cast<int>(boids.size()) - param.numberOfBoids;
    if (dif < 0) {
        for (int i = 0; i < -dif; i++) {
            boids.push_back(Boid3D());
            }
        } 
    else if (dif > 0) {
        for (int i = 0; i < dif; i++) {
            boids.pop_back();
            }
        }
    }

    void setVAO();

    void drawBoids(GLuint vao, std::vector<Boid3D>& boids, Camera::Trackball trackball, glm::mat4 normalMatrix, glm::mat4 projectionMatrix, ParamBoids3D& param, Window3D& window);
};