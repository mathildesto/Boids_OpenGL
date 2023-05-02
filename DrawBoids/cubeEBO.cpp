// #include <vcruntime.h>
// #include <cstddef> // For offsetof()
// #include <glm/gtc/random.hpp>
// #include "Camera/Freefly.h"
// #include "Camera/Trackball.h"
// #include "glimac/common.hpp"
// #include "glimac/sphere_vertices.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
// #include "glm/ext/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"
// #include "p6/p6.h"
// #include "boids3D/boid3D.hpp"

// int main()
// {

//     auto ctx       = p6::Context{{1280, 720, "TP6"}};
//     ctx.maximize_window();

//     auto trackball = Camera::Trackball{};
//     ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
//         trackball.moveFront(scroll.dy);
//     };
//     ctx.mouse_dragged = [&](p6::MouseDrag drag) {
//         trackball.rotateUp(drag.delta.y * 100);
//         trackball.rotateLeft(drag.delta.x * 100);
//     };


//    /** Shader **/
//    const p6::Shader shader = p6::load_shader(
//        "shaders/3D_light.vs.glsl",
//        "shaders/normals.fs.glsl"
//    );

//    const GLint uMVPMatrix            = glGetUniformLocation(shader.id(), "uMVPMatrix");
//    const GLint uMVMatrix             = glGetUniformLocation(shader.id(), "uMVMatrix");
//    const GLint uNormalMatrix         = glGetUniformLocation(shader.id(), "uNormalMatrix");

//    glEnable(GL_DEPTH_TEST); // 3D
//    glEnable(GL_BLEND); //transparancy
// //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    GLuint cubeVBO, cubeVAO;

//     Window3D window;

// // Cube vertices
// std::vector<glm::vec3> cubeVertices {
//     glm::vec3(window.WINDOW_MIN_X, window.WINDOW_MIN_Y, window.WINDOW_MIN_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MIN_Y, window.WINDOW_MIN_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MAX_Y, window.WINDOW_MIN_Z), glm::vec3(window.WINDOW_MIN_X,  window.WINDOW_MAX_Y, window.WINDOW_MIN_Z),
//     glm::vec3(window.WINDOW_MIN_X, window.WINDOW_MIN_Y,  window.WINDOW_MAX_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MIN_Y,  window.WINDOW_MAX_Z), glm::vec3( window.WINDOW_MAX_X, window.WINDOW_MAX_Y,  window.WINDOW_MAX_Z), glm::vec3(window.WINDOW_MIN_X,  window.WINDOW_MAX_Y, window.WINDOW_MAX_Z)
// };

// // Cube indices
// std::vector<GLuint> cubeIndices {
//     0, 1, 2, 2, 3, 0,
//     1, 5, 6, 6, 2, 1,
//     5, 4, 7, 7, 6, 5,
//     4, 0, 3, 3, 7, 4,
//     3, 2, 6, 6, 7, 3,
//     4, 5, 1, 1, 0, 4
// };

// // VBO
// glGenBuffers(1, &cubeVBO);
// glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
// glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(glm::vec3), cubeVertices.data(), GL_STATIC_DRAW);

// //EBO
// GLuint cubeEBO;
// glGenBuffers(1, &cubeEBO);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(GLuint), cubeIndices.data(), GL_STATIC_DRAW);

// // VAO
// glGenVertexArrays(1, &cubeVAO);
// glBindVertexArray(cubeVAO);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

// const GLuint VERTEX_ATTR_POSITION = 10;
// glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
// glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


// const GLuint VERTEX_ATTR_NORMAL = 11;
// glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
// glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

// const GLuint VERTEX_ATTR_TEXCOORDS = 12;
// glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
// glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

// // Unbind VAO and VBO
// glBindVertexArray(0);
// glBindBuffer(GL_ARRAY_BUFFER, 0);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



//    ctx.update = [&]() {
//         shader.use();
//         auto modelViewMatrix  = glm::translate(trackball.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
//         auto projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); 
//         auto normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

//         glClearColor(0.f, 0.f, 0.f, 1.f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         glBindVertexArray(cubeVAO);

//         glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
//         glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
//         glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

//         glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);

//         glBindVertexArray(0);
//    };

//    // Should be done last. It starts the infinite loop.
//    ctx.start();

//    glDeleteBuffers(1, &cubeVBO); // 1 car 1 seul vbo
//    glDeleteVertexArrays(1, &cubeVAO);
// }
