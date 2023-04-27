#include <vcruntime.h>
#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include "Camera/Freefly.h"
#include "Camera/Trackball.h"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "boids3D/boid3D.hpp"

static void render_gui(ParamBoids3D& param, Window3D& window){

        ImGui::SliderInt("numberOfBoids", &param.numberOfBoids, 2, 100);

        ImGui::SliderFloat("visualRange", &param.visualRange, 0.f, 2.0f);
        ImGui::SliderFloat("ProtectedRange", &param.protectedRange, 0.f, 1.f);

        ImGui::SliderFloat("turnfactor", &param.turnfactor, 0.f, 0.1f);
        ImGui::SliderFloat("centeringfactor", &param.centeringfactor, 0.f, 0.001f);
        ImGui::SliderFloat("avoidfactor", &param.avoidfactor, 0.f, 0.1f);
        ImGui::SliderFloat("matchingfactor", &param.matchingfactor, 0.f, 0.1f);

        ImGui::SliderFloat("MaxSpeed", &param.maxspeed, 0.f, 0.05f);
        ImGui::SliderFloat("MinSpeed", &param.minspeed, 0.f, 0.05f);

        ImGui::SliderFloat("Size", &param.boidSize, 0.f, 0.5f);

        ImGui::SliderFloat("MIN_X", &window.WINDOW_MIN_X, -4.0f, -1.0f);
        ImGui::SliderFloat("MAX_X", &window.WINDOW_MAX_X, 1.f, 4.f);

        ImGui::SliderFloat("MIN_Y", &window.WINDOW_MIN_Y, -4.f, -1.f);
        ImGui::SliderFloat("MAX_Y", &window.WINDOW_MAX_Y, 1.f, 4.f);

        ImGui::SliderFloat("MIN_Z", &window.WINDOW_MIN_Z, -4.f, -1.f);
        ImGui::SliderFloat("MAX_Z", &window.WINDOW_MAX_Z, 1.f, 4.f);
}

void resize(ParamBoids3D& param, std::vector<Boid3D> & boids){
    int dif = static_cast<int>(boids.size()) - param.numberOfBoids;
    if (dif < 0) {
        for (int i = 0; i < -dif; i++) {
            boids.push_back(Boid3D());
        }
    } else if (dif > 0) {
        for (int i = 0; i < dif; i++) {
            boids.pop_back();
        }
    }
}

int main()
{

    auto trackball = Camera::Trackball{};
    auto ctx       = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

    ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
        trackball.moveFront(scroll.dy);
    };
    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        trackball.rotateUp(drag.delta.y * 100);
        trackball.rotateLeft(drag.delta.x * 100);
    };


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

    ParamBoids3D param1;
    Window3D window1;
    std::vector<Boid3D> boids1 = initialise_positions(10);

    
    ctx.imgui                 = [&]() {
        ImGui::Begin("Test");
        render_gui(param1, window1);
        ImGui::End();
        ImGui::ShowDemoWindow();
    };

   ctx.update = [&]() {
        shader.use();
        auto modelViewMatrix  = glm::translate(trackball.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        auto projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); 
        auto normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        glUniform3fv(uKdVector, 1, glm::value_ptr(glm::vec3{0.5, 0.5, 0.5}));
        glUniform3fv(uKsVector, 1, glm::value_ptr(glm::vec3{0.5, 0.5, 0.5}));
        glUniform1f(uShininessFloat, 1.f);
        glUniform3fv(uLightDirVector, 1, glm::value_ptr(glm::vec3(glm::mat4{1} * glm::vec4{1.f,1.f,1.f, 1.f}))); 
        //glUniform3fv(uLightDirVector, 1, glm::value_ptr(glm::vec3(freefly.getViewMatrix() * glm::vec4{1.f,1.f,1.f, 1.f}))); // mat4 -> vec3 ???

        glUniform3fv(uLightIntensityVector, 1, glm::value_ptr(glm::vec3{1.f, 1.f, 1.f}));


        for (size_t i = 0; i < boids1.size(); i++)
        {
            // modelViewMatrix = glm::translate(glm::mat4{1}, glm::vec3(0.f, 0.f, -5.f));
            modelViewMatrix = glm::translate(trackball.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
            modelViewMatrix = glm::translate(modelViewMatrix, boids1[i].position);
            modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3{param1.boidSize});
            normalMatrix    = glm::transpose(glm::inverse(modelViewMatrix));

           glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
           glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
           glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

           glDrawArrays(GL_TRIANGLES, 0, vertices.size());
       }

        update_position(boids1, window1, param1);

        //Variation du nombre de boids
        resize(param1, boids1);

       glBindVertexArray(0);
   };

   // Should be done last. It starts the infinite loop.
   ctx.start();

   glDeleteBuffers(1, &vbo); // 1 car 1 seul vbo
   glDeleteVertexArrays(1, &vao);
}