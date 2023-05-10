#pragma once
#include "p6/p6.h"
#include <iostream>
#include<cmath>

class TextureLoading{
    public:
    static GLuint LoadTexture(const GLchar *path){
        const auto image_texture = p6::load_image_buffer(path);
        
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_texture.width(), image_texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_texture.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        return textureID;
    }

     static GLuint LoadCubemap( std::vector<const GLchar * > faces)
    {
        GLuint textureID;
        glGenTextures( 1, &textureID );
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );

        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

        
        for ( GLuint i = 0; i < faces.size( ); i++ )
        {
            const auto image_texture =  p6::load_image_buffer(faces[i]);
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image_texture.width(), image_texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_texture.data() );
        }

        glBindTexture( GL_TEXTURE_CUBE_MAP, 0);
        
        return textureID;
    }
};