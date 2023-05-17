#include <iostream>
#include <vector>
#include <unordered_set>
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include <cam/Freefly.h>

#include "tinyobjloader/tiny_obj_loader.h"
#include "glimac/common.hpp"

struct Program {
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

    Program()
        : m_Program(p6::load_shader("shaders/3D.vs.glsl", "shaders/directionalLight.fs.glsl"))
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

    }
};

int main()
{
auto freefly = Camera::Freefly{};
    auto ctx     = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

    glEnable(GL_DEPTH_TEST);

    Program program{};

    // std::string inputfile = "assets/models/cube.obj";
    std::string inputfile = "assets/models/12265_Fish_v1_L2.obj";
    // std::string inputfile = "assets/models/fish.obj";
    // std::string inputfile = "assets/models/cube2triangulate.obj";
    // std::string inputfile = "assets/models/cube2.obj";
    // std::string inputfile = "assets/models/cube_test.obj";
    // // std::string inputfile = "assets/models/cornell_box.obj";

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
    
    //////Texture

    GLuint textureID = 0;
    const auto texture = p6::load_image_buffer("assets/models/fish.jpg");

    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint vao, vbo, ibo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * sizeof(float), &attrib.vertices[0], GL_STATIC_DRAW);

    std::vector<unsigned int> indices;
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


    /* Loop until the user closes the window */
    ctx.update = [&]() {

        auto       modelViewMatrix  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        auto const projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); // fov, aspect ratio, near, far
        auto const normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.m_Program.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glUniform1i(program.uTexture, 0);

        glBindVertexArray(vao);

        glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
        glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glUniform3fv(program.uKa, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
        glUniform3fv(program.uKd, 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
        glUniform3fv(program.uKs, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
        glUniform1f(program.uShininess, 0.6);

        glUniform3fv(program.uLightPos_vs, 1, glm::value_ptr(glm::vec3(-3, -3, -3)));
        glUniform3fv(program.uLightIntensity, 1, glm::value_ptr(glm::vec3(1, 1, 1)));

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


        glBindVertexArray(0);

    freefly.handleEvent(ctx);

    };

    ctx.start();

    return 0;
}