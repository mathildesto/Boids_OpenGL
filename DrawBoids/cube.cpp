// #include <vcruntime.h>
// #include <cstddef> // For offsetof()
// #include <glm/gtc/random.hpp>
// #include "Cam/Freefly.h"
// #include "glimac/common.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
// #include "glm/ext/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"
// #include "p6/p6.h"
// #include "Aquarium/texture.hpp"

// int main()
// {

//     auto freefly = Camera::Freefly{};
//     auto ctx     = p6::Context{{1280, 720, "TP6"}};
//     ctx.maximize_window();

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

//     const p6::Shader shader = p6::load_shader(
//     "shaders/cube.vs.glsl",
//     "shaders/cube.fs.glsl"
//    );
//     glEnable(GL_DEPTH_TEST); // 3D
//     glEnable(GL_BLEND); 
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

//     // use with Perspective Projection
//     GLfloat vertices[] = {
//         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
//         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//     };


//     GLuint cubeVBO, cubeVAO;
//     glGenVertexArrays(1, &cubeVAO);
//     glGenBuffers(1, &cubeVBO);

//     glBindVertexArray(cubeVAO);

//     glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    
//     //position attribute
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*) 0);
//     glEnableVertexAttribArray(0);

//     //texture coordinate attribute
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
//     glEnableVertexAttribArray(2);

//     glBindVertexArray(0); //UnbindVAO

//     GLuint cubeTexture = TextureLoading::LoadTexture("assets/textures/water.jpg");
    
//     // glm::mat4 projection = glm::perspective(45.0f, ctx.aspect_ratio(), .1f, 100.f);
//     auto projection = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); 

//    ctx.update = [&]() {
//         glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         shader.use();

//         // Bind Textures using texture units
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, cubeTexture);
//         glUniform1i (glGetUniformLocation(shader.id(), "ourTexture1"), 0);


//         // auto modelViewMatrix  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
//         // auto normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

//         // Create transformations
//         glm::mat4 model = glm::mat4( 1.0f );
//         glm::mat4 view = glm::mat4( 1.0f );
//         // model = glm::rotate( model, ( GLfloat)glfwGetTime( ) * 1.0f, glm::vec3( 0.5f, 1.0f, 0.0f ) ); // use with perspective projection
//         // model  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
//         // view = glm::translate( view, glm::vec3( 0.0f, 0.0f, -3.0f ) ); // use with perspective projection
//         view  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));

        
//         // Get their uniform location
//         GLint modelLoc = glGetUniformLocation(shader.id(), "model" );
//         GLint viewLoc = glGetUniformLocation( shader.id(), "view" );
//         GLint projLoc = glGetUniformLocation(shader.id(), "projection" );
//         // Pass them to the shaders
//         glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
//         glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
//         glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );

//         glBindVertexArray(cubeVAO);
//         glDrawArrays(GL_TRIANGLES,0, 36);
//         glBindVertexArray(0);
//    };

//    // Should be done last. It starts the infinite loop.
//    ctx.start();

//    glDeleteBuffers(1, &cubeVBO); // 1 car 1 seul vbo
//    glDeleteVertexArrays(1, &cubeVAO);
// }
