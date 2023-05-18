#pragma once
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
#include "OpenGL_program/Texture_program.hpp"


struct FishBoidProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uTexture;

    GLint uKa;
    GLint uKd;
    GLint uKs;
    GLint uShininess;
    GLint uLightPos_vs;
    GLint uLightIntensity;

    GLuint vbo, vao, ibo, fishTexture;

    std::vector<unsigned int> indices;

    FishBoidProgram() : m_Program(p6::load_shader("shaders/3D.vs.glsl", "shaders/directionalLight.fs.glsl"))
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");

        uTexture = glGetUniformLocation(m_Program.id(), "uTexture");

        uKa             = glGetUniformLocation(m_Program.id(), "uKa");
        uKd             = glGetUniformLocation(m_Program.id(), "uKd");
        uKs             = glGetUniformLocation(m_Program.id(), "uKs");
        uShininess      = glGetUniformLocation(m_Program.id(), "uShininess");
        uLightPos_vs    = glGetUniformLocation(m_Program.id(), "uLightPos_vs");
        uLightIntensity = glGetUniformLocation(m_Program.id(), "uLightIntensity");

        fishTexture = TextureLoading::LoadImageTexture("assets/models/fish.jpg");

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

    void setVAO(){

        std::string inputfile = "assets/models/12265_Fish_v1_L2.obj";

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
    glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * sizeof(float), &attrib.vertices[0], GL_STATIC_DRAW);

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            indices.push_back(index.vertex_index);
        }
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set up the vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(6 * sizeof(float))); // texture coordinate

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

        };

    void draw(Camera::Freefly freefly, std::vector<Boid3D>& boids, p6::Context &ctx, ParamBoids3D& param,  Window3D& window){
        
        auto       modelViewMatrix  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.5f));
        auto const projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); // fov, aspect ratio, near, far
        auto const normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));
    

        m_Program.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fishTexture);

        glUniform1i(uTexture, 0);

        glBindVertexArray(vao);
        
        glUniform3fv(uKa, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
        glUniform3fv(uKd, 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
        glUniform3fv(uKs, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
        glUniform1f(uShininess, 0.6);

        glUniform3fv(uLightPos_vs, 1, glm::value_ptr(glm::vec3(-3, -3, -3)));
        glUniform3fv(uLightIntensity, 1, glm::value_ptr(glm::vec3(1, 1, 1)));

        
        for (size_t i = 0; i < boids.size(); i++)
        {
            // modelViewMatrix = glm::translate(glm::mat4{1}, glm::vec3(0.f, 0.f, -5.f));
            modelViewMatrix = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
            modelViewMatrix = glm::translate(modelViewMatrix, boids[i].position);
            // modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{param.boidSize});
                        modelViewMatrix = glm::rotate(modelViewMatrix, 300.f, glm::vec3(1.0, 0.0, 0.0));

            modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{0.001f});


            // normalMatrix    = glm::transpose(glm::inverse(modelViewMatrix));

           glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
           glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
           glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
       }

        update_position(boids,window, param);

        //Variation du nombre de boids
        resize(param, boids);

        glBindVertexArray(0);
    };
};