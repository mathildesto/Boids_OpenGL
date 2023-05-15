#include "Cam/Freefly.h"
#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct EarthProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.id(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.id(), "uCloudTexture");
    }
};

struct MoonProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/earth.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uTexture      = glGetUniformLocation(m_Program.id(), "uTexture");
    }
};

int main()
{
    auto freefly = Camera::Freefly{};
    auto ctx     = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

    ctx.mouse_moved = [&](p6::MouseMove move) {
      freefly.rotateLeft(move.delta.x * 100);
      freefly.rotateUp(move.delta.y * 100);
    };

    ctx.key_repeated = [&](p6::Key key) {
        const float step = .05f;

        switch(key.physical) {
        case GLFW_KEY_W:
            freefly.moveFront(step);
            break;
        case GLFW_KEY_S:
            freefly.moveFront(-step);
            break;
        case GLFW_KEY_A:
            freefly.moveLeft(-step);
            break;
        case GLFW_KEY_D:
            freefly.moveLeft(step);
            break;
        default:

            break;
        }

    };

    glEnable(GL_DEPTH_TEST); // 3D

    /** Shader **/
    EarthProgram earthProgram{};
    MoonProgram  moonProgram{};

    GLuint earthmap = 0;
    GLuint moonmap  = 0;
    GLuint cloudmap = 0;
    GLuint vbo      = 0;
    GLuint vao      = 0;

    /** Texture 1**/
    glGenTextures(1, &earthmap);
    glBindTexture(GL_TEXTURE_2D, earthmap);

    const auto earth_map_texture = p6::load_image_buffer("assets/textures/EarthMap.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earth_map_texture.width(), earth_map_texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, earth_map_texture.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    /** Texture 2**/
    glGenTextures(1, &moonmap);
    glBindTexture(GL_TEXTURE_2D, moonmap);

    const auto moon_map_texture = p6::load_image_buffer("assets/textures/MoonMap.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moon_map_texture.width(), moon_map_texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, moon_map_texture.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    /** Texture 3**/
    glGenTextures(1, &cloudmap);
    glBindTexture(GL_TEXTURE_2D, cloudmap);

    const auto cloud_map_texture = p6::load_image_buffer("assets/textures/CloudMap.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloud_map_texture.width(), cloud_map_texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, cloud_map_texture.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    /** VBO & VAO **/
    glGenBuffers(1, &vbo); // 1 car un seul vbo

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);
    for (int i = 0; i<vertices.size(); i++){
        std::cout<<"vertices"<<i<<":"<<&vertices[i]<<std::endl;
        std::cout<<"position"<<i<<":"<<&vertices[i].position<<std::endl;
        std::cout<<"normal"<<i<<":"<<&vertices[i].normal<<std::endl;
        std::cout<<"texCoords"<<i<<":"<<&vertices[i].texCoords<<std::endl;
    }

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /** VAO **/
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION   = 10;
    const GLuint VERTEX_ATTR_NORMAL     = 11;
    const GLuint VERTEX_ATTR_TEX_COORDS = 12;

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

    std::vector<glm::vec3> randoms_direction;
    std::vector<glm::vec3> randoms_position;
    for (int i = 0; i < 32; i++)
    {
        randoms_direction.push_back(glm::sphericalRand(1.f));
        randoms_position.emplace_back(p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f));
    }

    ctx.update = [&]() {
        auto       modelViewMatrix  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        auto const projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); // fov, aspect ratio, near, far
        auto const normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        earthProgram.m_Program.use();
        glBindTexture(GL_TEXTURE_2D, earthmap);

        // Main sphere
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthmap); // la texture earthTexture est bindée sur l'unité GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cloudmap); // la texture cloudTexture est bindée sur l'unité GL_TEXTURE1

        glUniform1i(earthProgram.uEarthTexture, 0);
        glUniform1i(earthProgram.uCloudTexture, 1);

        // modelViewMatrix = glm::rotate(modelViewMatrix, ctx.time(), glm::vec3{1.f, 0.f, 0.f});
        glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // 3 car 3 sommets

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE1

        moonProgram.m_Program.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonmap);
        glUniform1i(moonProgram.uTexture, 0);

        for (int i = 0; i < 32; i++)
        {
            //modelViewMatrix = glm::translate(glm::mat4{1}, glm::vec3(0.f, 0.f, -5.f));
            modelViewMatrix = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
            modelViewMatrix = glm::rotate(modelViewMatrix, ctx.time(), randoms_direction[i]);
            modelViewMatrix = glm::translate(modelViewMatrix, randoms_position[i]);
            modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{0.2f});

            glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
            glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo); // 1 car 1 seul vbo
    glDeleteVertexArrays(1, &vao);
}