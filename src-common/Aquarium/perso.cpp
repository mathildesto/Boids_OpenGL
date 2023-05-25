#include "Aquarium/perso.hpp"
#include "boids3D/boid3D.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <iostream>
#include "Cam/Freefly.h"
#include "Cam/Trackball.h"
#include<cmath>
#include "glimac/sphere_vertices.hpp"
#include "perso.hpp"
#include "glm/gtc/matrix_transform.hpp"

    void Personnage::setVAO(){
        const GLuint VERTEX_ATTR_POSITION   = 10;
        const GLuint VERTEX_ATTR_NORMAL     = 11;
        const GLuint VERTEX_ATTR_TEX_COORDS = 12;

        /** VBO & VAO **/
        glGenBuffers(1, &vbo); // 1 car un seul vbo

        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /** VAO **/
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Bind VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Position (VEC3)
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glVertexAttribPointer(
            VERTEX_ATTR_POSITION,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glimac::ShapeVertex),
            (void*)offsetof(glimac::ShapeVertex, position)
        );

        // Normal (VEC3)
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glVertexAttribPointer(
            VERTEX_ATTR_NORMAL,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glimac::ShapeVertex),
            (void*)offsetof(glimac::ShapeVertex, normal)
        );

        // Tex Coords (VEC2)
        glEnableVertexAttribArray(VERTEX_ATTR_TEX_COORDS);
        glVertexAttribPointer(
            VERTEX_ATTR_TEX_COORDS,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glimac::ShapeVertex),
            (void*)offsetof(glimac::ShapeVertex, texCoords)
        );

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        /** FIN VBO **/

        glBindVertexArray(0);
        /** FIN VAO **/
    }

    void Personnage::updatePosition(Camera::Freefly& camera) {
        float distanceFromCharacter = 0.5;

        // Mise à jour de la position du personnage avec la position calculée de la caméra
        _position = camera._position + camera._front * distanceFromCharacter;

    }

    void Personnage::draw(Camera::Freefly &freefly, glm::mat4 &projectionMatrix){
        m_Program.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(uTexture, 0);

        auto modelViewMatrix  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        auto const normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

        modelViewMatrix = glm::translate(modelViewMatrix,  _position);
        modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{0.07f});

       glBindVertexArray(vao);

       glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
       glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
       glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glUniform3fv(uKdVector, 1, glm::value_ptr(glm::vec3{0.5, 0.5, 0.5}));
        glUniform3fv(uKsVector, 1, glm::value_ptr(glm::vec3{0.5, 0.5, 0.5}));
        glUniform1f(uShininessFloat, 1.f);
        glUniform3fv(uLightDirVector, 1, glm::value_ptr(glm::vec3(glm::mat4{1} * glm::vec4{1.f,1.f,1.f, 1.f}))); 
        glUniform3fv(uLightIntensityVector, 1, glm::value_ptr(glm::vec3{1.f, 1.f, 1.f}));

       glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // 3 car 3 sommets

       updatePosition(freefly);
    }
