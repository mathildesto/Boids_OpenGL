#include <vcruntime.h>
#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include "Cam/Freefly.h"
#include "Cam/Trackball.h"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "boids3D/boid3D.hpp"
#include "boids3D/3DBoidsProgram.hpp"
#include "Aquarium/cubeEBO.hpp"
#include "Aquarium/cube.hpp"
#include "Aquarium/skybox.hpp"

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
    
    auto ctx     = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

    auto trackball = Camera::Trackball{};
    ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
        trackball.moveFront(scroll.dy);
    };
    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        trackball.rotateUp(drag.delta.y * 100);
        trackball.rotateLeft(drag.delta.x * 100);
    };
    
    // auto freefly = Camera::Freefly{};
    // ctx.mouse_moved = [&](p6::MouseMove move) {
    //   freefly.rotateLeft(move.delta.x * 100);
    //   freefly.rotateUp(move.delta.y * 100);
    // };

    // ctx.key_repeated = [&](p6::Key key) {
    //     const float step = .05f;

    //     switch(key.physical) {
    //     case GLFW_KEY_W:
    //         freefly.moveFront(step);
    //         break;
    //     case GLFW_KEY_S:
    //         freefly.moveFront(-step);
    //         break;
    //     case GLFW_KEY_A:
    //         freefly.moveLeft(-step);
    //         break;
    //     case GLFW_KEY_D:
    //         freefly.moveLeft(step);
    //         break;
    //     default:

    //         break;
    //     }

    // };

    Window3D window;

    // CubeEBOProgram cubeProgram{window};
    BoidsProgram boidsProgram{};
    CubeProgram aquarium;
    SkyboxProgram skybox;


    

   glEnable(GL_DEPTH_TEST); // 3D

    // cubeProgram.setVAO();
    boidsProgram.setVAO();
    aquarium.setVAO();
    skybox.setVAO();


   /////////////////////////////////////////////////////////////////////////////////////////

    ParamBoids3D param;
    std::vector<Boid3D> boids = initialise_positions(10);

    
    ctx.imgui                 = [&]() {
        ImGui::Begin("Test");
        render_gui(param, window);
        ImGui::End();
        ImGui::ShowDemoWindow();
    };
    
    auto projection = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); 

    ctx.update = [&]() {
        // auto view  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        auto view  = glm::translate(trackball.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
        auto normalMatrix     = glm::transpose(glm::inverse(view));

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // boidsProgram.drawBoids(boidsProgram.vao, boids, freefly, normalMatrix, projection, param, window);
        boidsProgram.drawBoids(boidsProgram.vao, boids, trackball, normalMatrix, projection, param, window);

        // cubeProgram.drawCube(cubeProgram.cubeVAO, freefly, normalMatrix, projection);
        // aquarium.drawCube(freefly, projection);
        // skybox.drawSkybox(freefly, projection);

        aquarium.drawCube(trackball, projection);
        skybox.drawSkybox(trackball, projection);


   };

   // Should be done last. It starts the infinite loop.
    ctx.start();
}