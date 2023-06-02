#pragma once
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <iostream>
#include "camera/Freefly.h"
#include<cmath>
#include "OpenGL_program/Texture_program.hpp"
#include "tinyobjloader/tiny_obj_loader.h"
#include "OpenGL_program/Texture_program.hpp"


struct PlantProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uTexture;

    GLint uKdVector;
    GLint uKsVector;
    GLint uShininessFloat; 
    GLint uLightDirVector;   
    GLint uLightPosVector; 
    GLint uLightIntensityVector;

    GLuint vbo, vao, ibo, textureID;

    std::vector<unsigned int> indices;

    // PlantProgram() : m_Program(p6::load_shader("shaders/3D_light.vs.glsl", "shaders/directionalLight.fs.glsl"))
    PlantProgram() : m_Program(p6::load_shader("shaders/3D_light.vs.glsl", "shaders/pointLight.fs.glsl"))

    {
        uMVPMatrix              = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix               = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix           = glGetUniformLocation(m_Program.id(), "uNormalMatrix");

        uKdVector               = glGetUniformLocation(m_Program.id(), "uKd");
        uKsVector               = glGetUniformLocation(m_Program.id(), "uKs");
        uShininessFloat         = glGetUniformLocation(m_Program.id(), "uShininess");
        // uLightDirVector         = glGetUniformLocation(m_Program.id(), "uLightDir_vs");
        uLightPosVector         = glGetUniformLocation(m_Program.id(), "uLightPos_vs");

        uLightIntensityVector   = glGetUniformLocation(m_Program.id(), "uLightIntensity");

        uTexture = glGetUniformLocation(m_Program.id(), "uTexture");

    }

    void setVAO(){

        std::string inputfile = "assets/models/Senecio_OBJ/senecio_1.obj";

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warn;
        std::string err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

        if (!warn.empty()) {
        std::cout << warn << std::endl;
        }

        if (!err.empty()) {
        std::cerr << err << std::endl;
        }

        if (!ret) {
        exit(1);
        }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * sizeof(float), attrib.vertices.data(), GL_STATIC_DRAW);


    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            indices.push_back(index.vertex_index);
        }
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set up the vertex attributes
    glEnableVertexAttribArray(10);
    glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(11);
    glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    glEnableVertexAttribArray(12);
    glVertexAttribPointer(12, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(6 * sizeof(float))); // texture coordinate

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

        };

    void draw(Camera::Freefly freefly, p6::Context &ctx){
        m_Program.use();

        glm::vec3 pointLightPos{0.f, 0.5f, 2.0};

        auto       modelViewMatrix  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, -0.5f, 2.0f));
        auto const projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); // fov, aspect ratio, near, far
        auto const normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));
        modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{0.1f});
        modelViewMatrix = glm::rotate(modelViewMatrix, 0.f, glm::vec3(0.0, 0.0, 1.0));

        glBindVertexArray(vao);

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glUniform3fv(uKdVector, 1, glm::value_ptr(glm::vec3{0.0, 1.0, 0.0}));
        glUniform3fv(uKsVector, 1, glm::value_ptr(glm::vec3{0.0, 1.0, 0.0}));
        glUniform1f(uShininessFloat, 1.f);
        glUniform3fv(uLightPosVector, 1, glm::value_ptr(glm::vec3(freefly.getViewMatrix() * glm::vec4{pointLightPos, 1.f}))); // mat4 -> vec3 ???

        glUniform3fv(uLightIntensityVector, 1, glm::value_ptr(glm::vec3{0.08f, 0.08f, 0.08f}));

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    };
};