#include <vcruntime.h>
#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include "Cam/Freefly.h"
// #include "Cam/camera.h"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "boids3D/boid3D.hpp"
#include "Aquarium/texture.hpp"


// // GLFW
// #include <GLFW/glfw3.h>

// // Function prototypes
// void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
// void MouseCallback( GLFWwindow *window, double xPos, double yPos );
// void DoMovement( );

//     // Camera
//     Cam camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
//     bool keys[1024];
//     GLfloat lastX = 400, lastY = 300;
//     bool firstMouse = true;

//     GLfloat deltaTime = 0.0f;
//     GLfloat lastFrame = 0.0f;

    

int main()
{

    auto ctx     = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

    
    auto freefly = Camera::Freefly{};
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
    // glEnable(GL_BLEND); 
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    
    const p6::Shader shader = p6::load_shader("shaders/cube.vs.glsl", "shaders/cube.fs.glsl");

    const p6::Shader skyboxShader = p6::load_shader("shaders/cube.vs.glsl", "shaders/cube.fs.glsl");

    // use with Perspective Projection
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

       // Setup cube VAO
    GLuint cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    //texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); //UnbindVAO

    // Setup skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays( 1, &skyboxVAO );
    glGenBuffers( 1, &skyboxVBO );
    glBindVertexArray( skyboxVAO );
    glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( skyboxVertices ), &skyboxVertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glBindVertexArray(0);

    GLuint cubeTexture = TextureLoading::LoadTexture("assets/textures/water.jpg");
    // GLuint cubeTexture = TextureLoading::LoadTexture("assets/texture/right.tga");


    // Cubemap (Skybox)
    std::vector<const GLchar*> faces;
    faces.push_back( "assets/textures/right.tga" );
    faces.push_back( "assets/textures/left.tga" );
    faces.push_back( "assets/textures/top.tga" );
    faces.push_back( "assets/textures/bottom.tga" );
    faces.push_back( "assets/textures/back.tga" );
    faces.push_back( "assets/textures/front.tga" );
    GLuint cubemapTexture = TextureLoading::LoadCubemap( faces );

    auto projection = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f); 
    // glm::mat4 projection = glm::perspective( camera.GetZoom( ), ctx.aspect_ratio(), 0.1f, 100.0f );

   ctx.update = [&]() {

        // // Set frame time
        // GLfloat currentFrame = glfwGetTime( );
        // deltaTime = currentFrame - lastFrame;
        // lastFrame = currentFrame;

        // // Check and call events
        // glfwPollEvents( );
        // DoMovement( );
        

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // glm::mat4 model;
        glm::mat4 model = glm::mat4( 1.0f );
        // glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view  = glm::translate(freefly.getViewMatrix(), glm::vec3(0.f, 0.f, 0.f));


        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glUniform1i (glGetUniformLocation(shader.id(), "ourTexture1"), 0);
        
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(shader.id(), "model" );
        GLint viewLoc = glGetUniformLocation( shader.id(), "view" );
        GLint projLoc = glGetUniformLocation(shader.id(), "projection" );

        // Pass them to the shaders
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );

        glBindVertexArray(cubeVAO);

        // Calculate the model matrix for each object and pass it to shader before drawing
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );

             
        // Draw skybox as last
        glDepthFunc( GL_LEQUAL );  // Change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use( );
        view = glm::mat4( glm::mat3( freefly.getViewMatrix()));	// Remove any translation component of the view matrix
        // view = glm::mat4( glm::mat3( camera.GetViewMatrix( ) ) );	// Remove any translation component of the view matrix

        glUniformMatrix4fv( glGetUniformLocation( skyboxShader.id(), "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( glGetUniformLocation( skyboxShader.id(), "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        
        // skybox cube
        glBindVertexArray( skyboxVAO );
        glBindTexture( GL_TEXTURE_CUBE_MAP, cubemapTexture );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );
        glDepthFunc( GL_LESS ); // Set depth function back to default

   };

   // Should be done last. It starts the infinite loop.
   ctx.start();

   glDeleteBuffers(1, &cubeVBO); // 1 car 1 seul vbo
   glDeleteVertexArrays(1, &cubeVAO);
}

// // Moves/alters the camera positions based on user input
// void DoMovement( )
// {
//     // Camera controls
//     if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
//     {
//         camera.ProcessKeyboard( FORWARD, deltaTime );
//     }
    
//     if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
//     {
//         camera.ProcessKeyboard( BACKWARD, deltaTime );
//     }
    
//     if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
//     {
//         camera.ProcessKeyboard( LEFT, deltaTime );
//     }
    
//     if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
//     {
//         camera.ProcessKeyboard( RIGHT, deltaTime );
//     }
// }

// // Is called whenever a key is pressed/released via GLFW
// void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
// {
//     if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
//     {
//         glfwSetWindowShouldClose(window, GL_TRUE);
//     }
    
//     if ( key >= 0 && key < 1024 )
//     {
//         if ( action == GLFW_PRESS )
//         {
//             keys[key] = true;
//         }
//         else if ( action == GLFW_RELEASE )
//         {
//             keys[key] = false;
//         }
//     }
// }

// void MouseCallback( GLFWwindow *window, double xPos, double yPos )
// {
//     if ( firstMouse )
//     {
//         lastX = xPos;
//         lastY = yPos;
//         firstMouse = false;
//     }
    
//     GLfloat xOffset = xPos - lastX;
//     GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
//     lastX = xPos;
//     lastY = yPos;
    
//     camera.ProcessMouseMovement( xOffset, yOffset );
// }


