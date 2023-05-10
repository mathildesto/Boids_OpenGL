#pragma once
#include "Cam/Freefly.h"
#include "boids3D/boid3D.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <iostream>
#include "Cam/Freefly.h"
#include "Cam/Trackball.h"
#include<cmath>
#include "glimac/sphere_vertices.hpp"
#include "OpenGL_program/Texture_program.hpp"


struct SkyboxProgram {
    p6::Shader skyboxShader;

    GLint viewLoc;
    GLint projLoc;
    GLint cubemapTexture;

    GLuint skyboxVAO, skyboxVBO;

    std::vector<const GLchar*> faces;

    GLfloat skyboxVertices[108] = {
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


    SkyboxProgram() : skyboxShader(p6::load_shader("shaders/skybox.vs.glsl", "shaders/skybox.fs.glsl"))
    {
        viewLoc    = glGetUniformLocation(skyboxShader.id(), "view");
        projLoc     = glGetUniformLocation(skyboxShader.id(), "projection");

        faces.push_back( "assets/textures/skybox/right.jpg" );
        faces.push_back( "assets/textures/skybox/left.jpg" );
        faces.push_back( "assets/textures/skybox/top.jpg" );
        faces.push_back( "assets/textures/skybox/bottom.jpg" );
        faces.push_back( "assets/textures/skybox/back.jpg" );
        faces.push_back( "assets/textures/skybox/front.jpg" );
    
        cubemapTexture = TextureLoading::LoadCubemap( faces );

    }

    void setVAO(){

    glGenVertexArrays( 1, &skyboxVAO );
    glGenBuffers( 1, &skyboxVBO );
    glBindVertexArray( skyboxVAO );
    glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( skyboxVertices ), &skyboxVertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glBindVertexArray(0);
        }

    void drawSkybox( Camera::Trackball trackball, glm::mat4 projection){

        // Draw skybox as last
        glDepthFunc( GL_LEQUAL );  // Change depth function so depth test passes when values are equal to depth buffer's content

        skyboxShader.use( );
        glm::mat4 view = glm::mat4( glm::mat3( trackball.getViewMatrix()));	// Remove any translation component of the view matrix
        // view = glm::mat4( glm::mat3( camera.GetViewMatrix( ) ) );	// Remove any translation component of the view matrix

        glUniformMatrix4fv( glGetUniformLocation( skyboxShader.id(), "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( glGetUniformLocation( skyboxShader.id(), "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        
        // skybox cube
        glBindVertexArray( skyboxVAO );
        glBindTexture( GL_TEXTURE_CUBE_MAP, cubemapTexture );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );
        glDepthFunc( GL_LESS ); // Set depth function back to default
    }
};