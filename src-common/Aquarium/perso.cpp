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
#include "tinyobjloader/tiny_obj_loader.h"


    void Personnage::setVAO(){

        std::string inputfile = "assets/models/shark.obj";

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

    std::vector<float> texCoords; // Ajout d'un vecteur pour stocker les coordonnées de texture

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            indices.push_back(index.vertex_index);

            // Récupérer les coordonnées de texture pour chaque sommet
            tinyobj::real_t tx = attrib.texcoords[2 * index.texcoord_index + 0];
            tinyobj::real_t ty = attrib.texcoords[2 * index.texcoord_index + 1];

            texCoords.push_back(tx);
            texCoords.push_back(ty);
        }
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
    // glVertexAttribPointer(12, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture coordinate

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

        };

    void Personnage::updatePosition(Camera::Freefly& camera) {
        float distanceFromCharacter = 0.8;

        // Mise à jour de la position du personnage avec la position calculée de la caméra
    _position = camera._position + camera._front * distanceFromCharacter;

    }

    void Personnage::draw(Camera::Freefly &freefly, glm::mat4 &projectionMatrix){
        m_Program.use();

        glm::vec3 pointLightPos = glm::vec3(_position.x, _position.y, _position.z);

        auto modelViewMatrix = glm::translate(freefly.getViewMatrix(),  glm::vec3(_position.x, _position.y -0.2, _position.z));

        auto const normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

        modelViewMatrix = glm::rotate(modelViewMatrix, freefly._phi, glm::vec3(0.0, 1.0, 0.0)); //y
        modelViewMatrix = glm::rotate(modelViewMatrix, -freefly._theta, glm::vec3(1.0, 0.0, 0.0)); //X
        modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{0.05f});
        modelViewMatrix = glm::rotate(modelViewMatrix, 0.f, glm::vec3(0.0, 0.0, 1.0));

        glBindVertexArray(vao);

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glUniform3fv(uKdVector, 1, glm::value_ptr(glm::vec3{0.3, 0.3, 0.5})); // Bleu/gris diffus
        glUniform3fv(uKsVector, 1, glm::value_ptr(glm::vec3{0.1, 0.1, 0.3})); // Bleu/gris spéculaire
        glUniform1f(uShininessFloat, 0.5f);
        // glUniform3fv(uLightDirVector, 1, glm::value_ptr(glm::vec3(glm::mat4{1} * glm::vec4{1.f,1.f,1.f, 1.f}))); 
        glUniform3fv(uLightPosVector, 1, glm::value_ptr(glm::vec3(modelViewMatrix * glm::vec4{pointLightPos, 1.f}))); 
        glUniform3fv(uLightIntensityVector, 1, glm::value_ptr(glm::vec3{0.008f, 0.008f, 0.008f}));

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

       updatePosition(freefly);
    }
