#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include "camera/Freefly.h"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

int main()
{
    auto freefly = Camera::Freefly{};
   freefly.moveFront(-5.f);
   auto ctx = p6::Context{{1280, 720, "TP8"}};
   ctx.maximize_window();

   /** Shader **/
   const p6::Shader shader = p6::load_shader(
       "shaders/3D_light.vs.glsl",
       "shaders/directionalLight.fs.glsl"
   );

   const GLint uMVPMatrix            = glGetUniformLocation(shader.id(), "uMVPMatrix");
   const GLint uMVMatrix             = glGetUniformLocation(shader.id(), "uMVMatrix");
   const GLint uNormalMatrix         = glGetUniformLocation(shader.id(), "uNormalMatrix");
   const GLint uKdVector             = glGetUniformLocation(shader.id(), "uKd");
   const GLint uKsVector             = glGetUniformLocation(shader.id(), "uKs");
   const GLint uShininessFloat       = glGetUniformLocation(shader.id(), "uShininess");
   const GLint uLightDirVector       = glGetUniformLocation(shader.id(), "uLightDir_vs");
   const GLint uLightIntensityVector = glGetUniformLocation(shader.id(), "uLightIntensity");

   ctx.mouse_moved = [&](p6::MouseMove move) {
       freefly.rotateLeft(move.delta.x * 100);
       freefly.rotateUp(move.delta.y * 100);
   };

   ctx.key_repeated = [&](p6::Key key) {
       const float step = .01f;

       switch (key.physical)
       {
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

   GLuint vbo = 0;
   GLuint vao = 0;

   /** VBO & VAO **/
   glGenBuffers(1, &vbo); // 1 car un seul vbo

   // VBO
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);
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
       shader.use();
       auto modelViewMatrix  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
       auto projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); // fov, aspect ratio, near, far
       auto normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

       glClearColor(0.f, 0.f, 0.f, 1.f);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       glBindVertexArray(vao);

       glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
       glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
       glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

       glUniform3fv(uKdVector, 1, glm::value_ptr(glm::vec3{0.5, 0.0, 0.0}));
       glUniform3fv(uKsVector, 1, glm::value_ptr(glm::vec3{0.5, 0.0, 0.0}));
       glUniform1f(uShininessFloat, 1.f);
       glUniform3fv(uLightDirVector, 1, glm::value_ptr(glm::vec3(freefly.getViewMatrix() * glm::vec4{2.f,2.f,2.f, 1.f}))); // mat4 -> vec3 ???
       glUniform3fv(uLightIntensityVector, 1, glm::value_ptr(glm::vec3{1.f, .3f, .5f}));

       glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // 3 car 3 sommets

       for (int i = 0; i < 32; i++)
       {
           // modelViewMatrix = glm::translate(glm::mat4{1}, glm::vec3(0.f, 0.f, -5.f));
           modelViewMatrix = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
           modelViewMatrix = glm::rotate(modelViewMatrix, ctx.time(), randoms_direction[i]);
           modelViewMatrix = glm::translate(modelViewMatrix, randoms_position[i]);
           modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{0.2f});
           normalMatrix    = glm::transpose(glm::inverse(modelViewMatrix));

           glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
           glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
           glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

           glDrawArrays(GL_TRIANGLES, 0, vertices.size());
       }

       glBindVertexArray(0);
   };

   // Should be done last. It starts the infinite loop.
   ctx.start();

   glDeleteBuffers(1, &vbo); // 1 car 1 seul vbo
   glDeleteVertexArrays(1, &vao);
}