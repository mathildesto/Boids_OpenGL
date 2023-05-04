#include <vcruntime.h>
#include <cstddef> // For offsetof()
#include <glm/gtc/random.hpp>
#include "Cam/Freefly.h"
#include "glimac/common.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "Aquarium/texture.hpp"

int main()
{

    auto ctx     = p6::Context{{1280, 720, "TP6"}};
    ctx.maximize_window();

//////////////////////camera/////////////////////////////
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
/////////////////////////////////////////////////////////////

    glEnable(GL_DEPTH_TEST); // 3D
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    const p6::Shader skyboxShader = p6::load_shader("shaders/skybox.vs.glsl", "shaders/skybox.fs.glsl");

    GLfloat skyboxVertices[] = {
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

    // Cubemap (Skybox)
    std::vector<const GLchar*> faces;
    faces.push_back( "assets/textures/skybox/right.jpg" );
    faces.push_back( "assets/textures/skybox/left.jpg" );
    faces.push_back( "assets/textures/skybox/top.jpg" );
    faces.push_back( "assets/textures/skybox/bottom.jpg" );
    faces.push_back( "assets/textures/skybox/back.jpg" );
    faces.push_back( "assets/textures/skybox/front.jpg" );

    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

   ctx.update = [&]() {

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
             
        // Draw skybox as last
        glDepthFunc( GL_LEQUAL );  // Change depth function so depth test passes when values are equal to depth buffer's content

        skyboxShader.use( );
        glUniform1i(glGetUniformLocation(skyboxShader.id(), "skybox"), 0);

        auto projection = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 1000.f); 
        glm::mat4 view = glm::mat4( glm::mat3( freefly.getViewMatrix()));	// Remove any translation component of the view matrix

        glUniformMatrix4fv( glGetUniformLocation( skyboxShader.id(), "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( glGetUniformLocation( skyboxShader.id(), "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        
        // skybox cube
        glBindVertexArray( skyboxVAO );
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_CUBE_MAP, cubemapTexture );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );
        glDepthFunc( GL_LESS ); // Set depth function back to default

   };

   // Should be done last. It starts the infinite loop.
   ctx.start();
}


