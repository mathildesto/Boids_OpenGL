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
#include "boids3D/3DBoidsProgram.hpp"
#include "Aquarium/cube.hpp"

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
}


int main()
{
    auto ctx       = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

    auto trackball = Camera::Trackball{};
    ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
        trackball.moveFront(scroll.dy);
    };
    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        trackball.rotateUp(drag.delta.y * 100);
        trackball.rotateLeft(drag.delta.x * 100);
    };

    Window3D window;

    CubeProgram cubeProgram{window};
    BoidsProgram boidsProgram{};
    

   glEnable(GL_DEPTH_TEST); // 3D

    const GLuint VERTEX_ATTR_POSITION   = 10;
   const GLuint VERTEX_ATTR_NORMAL     = 11;
   const GLuint VERTEX_ATTR_TEX_COORDS = 12;

cubeProgram.setVAO();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////BOIDS////////////////////////////////////////
boidsProgram.setVAO();

   /////////////////////////////////////////////////////////////////////////////////////////

    ParamBoids3D param;
    std::vector<Boid3D> boids = initialise_positions(10);

    
    ctx.imgui                 = [&]() {
        ImGui::Begin("Test");
        render_gui(param, window);
        ImGui::End();
        ImGui::ShowDemoWindow();
    };

   ctx.update = [&]() {
        auto modelViewMatrix  = glm::translate(trackball.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        auto projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); 
        auto normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        boidsProgram.drawBoids(boidsProgram.vao, boids, trackball, normalMatrix, projectionMatrix, param, window);

        ////////////////////////CUBE/////////////////////////////////////////
        cubeProgram.drawCube(cubeProgram.cubeVAO, trackball, normalMatrix, projectionMatrix);
        /////////////////////////////////////////////////////////////////////////////
   };

   // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &boidsProgram.vbo); // 1 car 1 seul vbo
    glDeleteVertexArrays(1, &boidsProgram.vao);
    glDeleteBuffers(1, &cubeProgram.cubeVBO); // 1 car 1 seul vbo
    glDeleteVertexArrays(1, &cubeProgram.cubeVAO);
}