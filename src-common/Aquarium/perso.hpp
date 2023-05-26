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
#include "OpenGL_program/Texture_program.hpp"
#include "tinyobjloader/tiny_obj_loader.h"



struct Personnage {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uTexture;

    GLint uKdVector;
    GLint uKsVector;
    GLint uShininessFloat; 
    GLint uLightDirVector;   
    GLint uLightIntensityVector;

    GLuint vbo, vao, ibo, textureID;

    // Spherical coordinate
    glm::vec3 _position{};

    std::vector<unsigned int> indices;

    Personnage(glm::vec3 position = {}) : _position(position), m_Program(p6::load_shader("shaders/3D_light.vs.glsl", "shaders/directionalLight.fs.glsl"))
    {
        uMVPMatrix              = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix               = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix           = glGetUniformLocation(m_Program.id(), "uNormalMatrix");

        uKdVector               = glGetUniformLocation(m_Program.id(), "uKd");
        uKsVector               = glGetUniformLocation(m_Program.id(), "uKs");
        uShininessFloat         = glGetUniformLocation(m_Program.id(), "uShininess");
        uLightDirVector         = glGetUniformLocation(m_Program.id(), "uLightDir_vs");
        uLightIntensityVector   = glGetUniformLocation(m_Program.id(), "uLightIntensity");

        uTexture = glGetUniformLocation(m_Program.id(), "uTexture");
        textureID = TextureLoading::LoadImageTexture("assets/textures/MoonMap.jpg");


    }

    void setVAO();

    void updatePosition(Camera::Freefly& camera);

    void draw(Camera::Freefly& freefly, glm::mat4 &projectionMatrix);


};