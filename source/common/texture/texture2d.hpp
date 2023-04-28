#pragma once

#include <glad/gl.h>

namespace our
{

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D
    {
        // The OpenGL object name of this texture
        GLuint name = 0;

    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name"
        Texture2D()
        {
            // Create a texture object
            glGenTextures(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D()
        {
            // Delete the texture object
            glDeleteTextures(1, &name);
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName()
        {
            // return the name of the texture object
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const
        {
            // Bind the texture object to GL_TEXTURE_2D
            glBindTexture(GL_TEXTURE_2D, name);
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind()
        {
            // Unbind the default texture to GL_TEXTURE_2D
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // Delete the copy constructor and assignment operator
        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
    };

}