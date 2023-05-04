// #include <vcruntime.h>
// #include <cstddef> // For offsetof()
// #include <glm/gtc/random.hpp>
// #include "Cam/Freefly.h"
// // #include "Cam/camera.h"
// #include "glimac/common.hpp"
// #include "glimac/sphere_vertices.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
// #include "glm/ext/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"
// #include "p6/p6.h"
// #include "boids3D/boid3D.hpp"
// #include "Aquarium/texture.hpp"
// #include "Aquarium/cube.hpp"
// #include "Aquarium/skybox.hpp"

    

// int main()
// {

//     auto ctx     = p6::Context{{1280, 720, "TP6"}};
//     ctx.maximize_window();

    
//     auto freefly = Camera::Freefly{};
//     ctx.mouse_moved = [&](p6::MouseMove move) {
//       freefly.rotateLeft(move.delta.x * 100);
//       freefly.rotateUp(move.delta.y * 100);
//     };

//     ctx.key_repeated = [&](p6::Key key) {
//         const float step = .05f;

//         switch(key.physical) {
//         case GLFW_KEY_W:
//             freefly.moveFront(step);
//             break;
//         case GLFW_KEY_S:
//             freefly.moveFront(-step);
//             break;
//         case GLFW_KEY_A:
//             freefly.moveLeft(-step);
//             break;
//         case GLFW_KEY_D:
//             freefly.moveLeft(step);
//             break;
//         default:

//             break;
//         }

//     };


//     glEnable(GL_DEPTH_TEST); // 3D

//     const p6::Shader shader = p6::load_shader("shaders/cube.vs.glsl", "shaders/cube.fs.glsl");

//     CubeProgram aquarium;
//     SkyboxProgram skybox;

//     aquarium.setVAO();
//     skybox.setVAO();

//     auto projection = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 1000.f); 

//    ctx.update = [&]() {
        
//         glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         aquarium.drawCube(freefly, projection);
//         skybox.drawSkybox(freefly, projection);

//    };

//    // Should be done last. It starts the infinite loop.
//    ctx.start();
// }



