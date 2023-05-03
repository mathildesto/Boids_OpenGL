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
// #include "Aquarium/texture.hpp"

// int main()
// {
// /**
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

//     **/

// /**
//    const p6::Shader skyboxShader = p6::load_shader(
//        "shaders/skybox.vs.glsl",
//        "shaders/skybox.fs.glsl"
//    );
//    **/

//       const p6::Shader shader = p6::load_shader(
//        "shaders/cube.vs.glsl",
//        "shaders/cube.fs.glsl"
//    );
//     glEnable(GL_DEPTH_TEST); // 3D
//     glEnable(GL_BLEND); 
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

//     GLfloat vertices[] = {
//     // x     y      z  
//     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
//      0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
//      0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
//     -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

//     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//      0.5f,  0.5f, 0.5f, 1.0f, 1.0f,
//      0.5f,  0.5f, 0.5f, 1.0f, 1.0f,
//     -0.5f,  0.5f, 0.5f, 0.0f, 1.0f,
//     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

//     -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
//     -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
//     -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

//     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
//     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
//     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//     0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
//     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

//     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
//      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//     -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
//     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

//     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
//      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//      0.5f, 0.5f,  0.5f, 1.0f, 0.0f,
//      0.5f, 0.5f,  0.5f, 1.0f, 0.0f,
//     -0.5f, 0.5f,  0.5f, 0.0f, 0.0f,
//     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
//    };

// /**
//   GLfloat skyboxVertices[] = {
//     -1.0f, 1.0f, -1.0f,
//     -1.0f, -1.0f, -1.0f,
//     1.0f, -1.0f, -1.0f,
//     1.0f, -1.0f, -1.0f,
//     1.0f, 1.0f, -1.0f,
//     -1.0f, 1.0f, -1.0f,

//     -1.0f, -1.0f, 1.0f,
//     -1.0f, -1.0f, -1.0f,
//     -1.0f, 1.0f, -1.0f,
//     -1.0f, 1.0f, -1.0f,
//     -1.0f, 1.0f, 1.0f,
//     -1.0f, -1.0f, 1.0f,

//     1.0f, -1.0f, -1.0f,
//     1.0f, -1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f, -1.0f,
//     1.0f, -1.0f, -1.0f,

//     -1.0f, -1.0f, 1.0f,
//     -1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f, -1.0f, 1.0f,
//     -1.0f, -1.0f, 1.0f,

//     -1.0f, 1.0f, -1.0f,
//     1.0f, 1.0f, -1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     -1.0f, 1.0f, 1.0f,
//     -1.0f, 1.0f, -1.0f,

//     -1.0f, -1.0f, -1.0f,
//     -1.0f, -1.0f, 1.0f,
//     1.0f, -1.0f, -1.0f,
//     1.0f, -1.0f, -1.0f,
//     -1.0f, -1.0f, 1.0f,
//     1.0f, -1.0f, 1.0f,
//    };
// **/

//     const GLuint VERTEX_ATTR_POSITION = 0;
//     const GLuint VERTEX_ATTR_TEXTURE = 1;
//     GLuint cubeVBO, cubeVAO;
//     glGenVertexArrays(1, &cubeVAO);
//     glGenBuffers(1, &cubeVBO);
//     glBindVertexArray(cubeVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    
//     //position attribute
//     glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
//     glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*) 0);
   
//     //texture coordinate attribute
//     glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
//     glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
   
//     glBindVertexArray(0); //UnbindVAO
// /**
//     GLuint skyboxVAO, skyboxVBO;
//     glGenVertexArrays(1, &skyboxVAO);
//     glGenBuffers(1, &skyboxVBO);
//     glBindVertexArray(skyboxVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
//     glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,3*sizeof(GLfloat), (GLvoid*) 0);
//     glBindVertexArray(0);

//     Load texture
//     GLuint cubeTexture = TextureLoading::LoadTexture("assets/textures/water.jpg");
// **/
//         GLuint cubeTexture;
//         glGenTextures(1, &cubeTexture);
//         glBindTexture(GL_TEXTURE_2D, cubeTexture);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         const auto image_texture = p6::load_image_buffer("assets/textures/water.jpg");
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_texture.width(), image_texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_texture.data());
//         glBindTexture(GL_TEXTURE_2D, 0);
        
//         GLint textureLoc = glGetUniformLocation(shader.id(), "texture1");



//    ctx.update = [&]() {
//         glClearColor(0.f, 0.f, 0.f, 1.f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, cubeTexture);
//         glUniform1i (textureLoc, 0);

//         shader.use();
        
//         glm::mat4 projection = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f);
//         glm::mat4 model = glm::rotate(model,(GLfloat)glfwGetTime()*1.0f, glm::vec3( 0.5f, 1.0f, 0.0f));
//         // view = glm::translate(trackball.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));
//         glm::mat4 view = glm::translate(view, glm::vec3(0.f, 0.f, -3.0f));

//         GLint modelLoc  = glGetUniformLocation (shader.id(), "model");
//         GLint viewLoc   = glGetUniformLocation (shader.id(), "view");
//         GLint projLoc   = glGetUniformLocation (shader.id(), "projection");

//         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//         glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//         glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

//         glBindVertexArray(cubeVAO);
//         glDrawArrays(GL_TRIANGLES,0, 36);

//         glBindVertexArray(0);
//    };

//    // Should be done last. It starts the infinite loop.
//    ctx.start();

//    glDeleteBuffers(1, &cubeVBO); // 1 car 1 seul vbo
//    glDeleteVertexArrays(1, &cubeVAO);
// }
