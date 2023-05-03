#pragma once
#include "boids3D/boid3D.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <iostream>
#include "Cam/Trackball.h"
#include<cmath>
#include "glimac/sphere_vertices.hpp"

struct CubeProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uWaterTexture;

    GLuint cubeVBO, cubeVAO, cubeEBO, waterTexture;

    std::vector<glm::vec3> cubeVertices;

    std::vector<GLuint> cubeIndices = {
        0, 1, 2, 2, 3, 0,
        1, 5, 6, 6, 2, 1,
        5, 4, 7, 7, 6, 5,
        4, 0, 3, 3, 7, 4,
        3, 2, 6, 6, 7, 3,
        4, 5, 1, 1, 0, 4
    };

    CubeProgram(Window3D& window) : m_Program(p6::load_shader("shaders/3D_light.vs.glsl", "shaders/normals.fs.glsl"))
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uWaterTexture      = glGetUniformLocation(m_Program.id(), "uTexture");

        cubeVertices = {
            glm::vec3(window.WINDOW_MIN_X, window.WINDOW_MIN_Y, window.WINDOW_MIN_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MIN_Y, window.WINDOW_MIN_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MAX_Y, window.WINDOW_MIN_Z), glm::vec3(window.WINDOW_MIN_X,  window.WINDOW_MAX_Y, window.WINDOW_MIN_Z),
            glm::vec3(window.WINDOW_MIN_X, window.WINDOW_MIN_Y,  window.WINDOW_MAX_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MIN_Y,  window.WINDOW_MAX_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MAX_Y,  window.WINDOW_MAX_Z), glm::vec3(window.WINDOW_MIN_X,  window.WINDOW_MAX_Y, window.WINDOW_MAX_Z)
        };
    }

    void setVAO(){

        const GLuint VERTEX_ATTR_POSITION   = 10;
        const GLuint VERTEX_ATTR_NORMAL     = 11;
        const GLuint VERTEX_ATTR_TEX_COORDS = 12;

        glGenTextures(1, &waterTexture);
        glBindTexture(GL_TEXTURE_2D, waterTexture);

        const auto water_map_texture = p6::load_image_buffer("assets/textures/water.jpg");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, water_map_texture.width(), water_map_texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, water_map_texture.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

        glBindTexture(GL_TEXTURE_2D, 0);
   
        // VBO
        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(glm::vec3), cubeVertices.data(), GL_STATIC_DRAW);

        //EBO
        glGenBuffers(1, &cubeEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(GLuint), cubeIndices.data(), GL_STATIC_DRAW);

        // VAO
        glGenVertexArrays(1, &cubeVAO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(VERTEX_ATTR_TEX_COORDS);
        glVertexAttribPointer(VERTEX_ATTR_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Unbind VAO and VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

    void drawCube(GLuint cubeVAO, Camera::Trackball trackball, glm::mat4 normalMatrix, glm::mat4 projectionMatrix){
                ////////////////////////CUBE/////////////////////////////////////////
        auto modelViewMatrixCube  = glm::translate(trackball.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        m_Program.use();
        
        glBindVertexArray(cubeVAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterTexture);
        glUniform1i(uWaterTexture, 0);


        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrixCube));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrixCube));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        /////////////////////////////////////////////////////////////////////////////
    }
};