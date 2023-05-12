// #include "tinyobjloader/tiny_obj_loader.h"

// #include <vcruntime.h>
// #include <cstddef> // For offsetof()
// #include <glm/gtc/random.hpp>
// #include "Cam/Freefly.h"
// #include "Cam/Trackball.h"
// #include "glimac/common.hpp"
// #include "glimac/sphere_vertices.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
// #include "glm/ext/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"
// #include "p6/p6.h"
// #include "boids3D/boid3D.hpp"
// #include "boids3D/3DBoidsProgram.hpp"
// #include "Aquarium/cubeEBO.hpp"
// #include "Aquarium/cube.hpp"
// #include "Aquarium/skybox.hpp"



// int main()
// {
    
//     auto ctx     = p6::Context{{1280, 720, "TP6"}};
//     ctx.maximize_window();

//     auto trackball = Camera::Trackball{};
//     ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
//         trackball.moveFront(scroll.dy);
//     };
//     ctx.mouse_dragged = [&](p6::MouseDrag drag) {
//         trackball.rotateUp(drag.delta.y * 100);
//         trackball.rotateLeft(drag.delta.x * 100);
//     };
    

//     std::string inputfile = "assets/models/cube.obj";
//     tinyobj::attrib_t attrib;
//     std::vector<tinyobj::shape_t> shapes;
//     std::vector<tinyobj::material_t> materials;

//     std::string warn;
//     std::string err;

//     materials.push_back(tinyobj::material_t{});

//     bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), "assets/models/");

//     if (!warn.empty()) {
//         std::cout << warn << std::endl;
//     }

//     if (!err.empty()) {
//         std::cerr << err << std::endl;
//     }

//     if (!ret) {
//         exit(1);
//     }

//         const p6::Shader fish_program = p6::load_shader("shaders/3D_light.vs.glsl", "shaders/directionalLight.fs.glsl");

//         const GLint uMVPMatrix            = glGetUniformLocation(fish_program.id(), "uMVPMatrix");
//         const GLint uMVMatrix             = glGetUniformLocation(fish_program.id(), "uMVMatrix");
//         const GLint uNormalMatrix         = glGetUniformLocation(fish_program.id(), "uNormalMatrix");
//         const GLint uKdVector             = glGetUniformLocation(fish_program.id(), "uKd");
//         const GLint uKsVector             = glGetUniformLocation(fish_program.id(), "uKs");
//         const GLint uShininessFloat       = glGetUniformLocation(fish_program.id(), "uShininess");
//         const GLint uLightPosVector       = glGetUniformLocation(fish_program.id(), "uLightPos_vs");
//         const GLint uLightIntensityVector = glGetUniformLocation(fish_program.id(), "uLightIntensity");

//         GLuint fishVBO, fishVAO;

        
//         const GLuint VERTEX_ATTR_POSITION   = 10;
//         const GLuint VERTEX_ATTR_NORMAL     = 11;
//         const GLuint VERTEX_ATTR_TEX_COORDS = 12;

//         /** fishVBO & fishVAO **/
//         glGenBuffers(1, &fishVBO); // 1 car un seul fishVBO

//         // fishVBO
//         glBindBuffer(GL_ARRAY_BUFFER, fishVBO);
//         // glBufferData(GL_ARRAY_BUFFER, shapes.size() * sizeof(glimac::ShapeVertex), shapes.data(), GL_STATIC_DRAW);
//         glBufferData(GL_ARRAY_BUFFER, shapes.size() * sizeof(tinyobj::shape_t), shapes.data(), GL_STATIC_DRAW);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);

//         /** fishVAO **/
//         glGenVertexArrays(1, &fishVAO);
//         glBindVertexArray(fishVAO);

//         // Bind fishVBO
//         glBindBuffer(GL_ARRAY_BUFFER, fishVBO);

//         // Position (VEC3)
//         glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
//         glVertexAttribPointer(
//             VERTEX_ATTR_POSITION,
//             3,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(tinyobj::shape_t),
//             (GLvoid*) 0);

//         // Normal (VEC3)
//         glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
//         glVertexAttribPointer(
//             VERTEX_ATTR_NORMAL,
//             3,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(tinyobj::shape_t),
//             (GLvoid*) 0);

//         // Tex Coords (VEC2)
//         glEnableVertexAttribArray(VERTEX_ATTR_TEX_COORDS);
//         glVertexAttribPointer(
//             VERTEX_ATTR_TEX_COORDS,
//             2,
//             GL_FLOAT,
//             GL_FALSE,
//             sizeof(tinyobj::shape_t),
//             (GLvoid*) 0);

//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         /** FIN fishVBO **/

//         glBindVertexArray(0);
//         /** FIN fishVAO **/


//    glEnable(GL_DEPTH_TEST); // 3D


//     glm::vec3              pointLightPos{10.f, 0.f, 10.f};


//     ctx.update = [&]() {
//         glClearColor(0.f, 0.f, 0.f, 1.f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         fish_program.use();

//         auto modelViewMatrix  = trackball.getViewMatrix();
//         auto projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); // fov, aspect ratio, near, far
//         auto normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

//         pointLightPos          = glm::vec4(pointLightPos, 1.0f);

//         glBindVertexArray(fishVAO);

//         glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
//         glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
//         glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

//         glUniform3fv(uKdVector, 1, glm::value_ptr(glm::vec3{0.5, 0.0, 0.2}));
//         glUniform3fv(uKsVector, 1, glm::value_ptr(glm::vec3{0.1, 0.5, 0.0}));
//         glUniform1f(uShininessFloat, 2.f);
//         glUniform3fv(uLightPosVector, 1, glm::value_ptr(glm::vec3(trackball.getViewMatrix() * glm::vec4{pointLightPos, 1.f}))); // mat4 -> vec3 ???
//         glUniform3fv(uLightIntensityVector, 1, glm::value_ptr(glm::vec3{1.f, .3f, .5f}));

//         glDrawArrays(GL_TRIANGLES, 0, shapes.size()); // 3 car 3 sommets

//    };

//    // Should be done last. It starts the infinite loop.
//     ctx.start();
// }