#include <iostream>
#include <vector>
#include <unordered_set>
#include "glimac/FreeflyCamera.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include <cam/Freefly.h>

#include "tinyobjloader/tiny_obj_loader.h"
#include "glimac/common.hpp"

int const window_width  = 1920;
int const window_height = 1080;

struct EarthProgram {
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

    EarthProgram()
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

    EarthProgram earth{};

    std::string inputfile = "assets/models/cube.obj";
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
    }

    if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();


    GLuint vao, vbo, ibo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * sizeof(float), &attrib.vertices[0], GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size() * sizeof(unsigned int), &shapes[0].mesh.indices[0], GL_STATIC_DRAW);
    
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

        earth.m_Program.use();

        glUniform1i(earth.uTexture, 0);

        glBindVertexArray(vao);

        glUniformMatrix4fv(earth.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
        glUniformMatrix4fv(earth.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(earth.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glUniform3fv(earth.uKa, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
        glUniform3fv(earth.uKd, 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
        glUniform3fv(earth.uKs, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
        glUniform1f(earth.uShininess, 0.6);

        glUniform3fv(earth.uLightPos_vs, 1, glm::value_ptr(glm::vec3(-3, -3, -3)));
        glUniform3fv(earth.uLightIntensity, 1, glm::value_ptr(glm::vec3(1, 1, 1)));

        glDrawArrays(GL_TRIANGLES, 0, attrib.vertices.size());

        // glDrawElements(GL_TRIANGLES, shapes[0].mesh.indices.size(), GL_UNSIGNED_INT, 0);


        glBindVertexArray(0);

    freefly.handleEvent(ctx);

    };

    ctx.start();

    return 0;
}