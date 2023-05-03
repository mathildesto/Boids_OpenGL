#pragma once
#include "boids3D/boid3D.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include <iostream>
#include "Cam/Trackball.h"
#include<cmath>
#include "glimac/sphere_vertices.hpp"

class TextureLoading{
    public:
    static GLuint LoadTexture(const GLchar *path){
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const auto image_texture = p6::load_image_buffer(path);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_texture.width(), image_texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_texture.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        return textureID;
    }
};