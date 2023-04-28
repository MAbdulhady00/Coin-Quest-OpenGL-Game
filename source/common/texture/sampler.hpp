#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

namespace our
{

    // This class defined an OpenGL sampler
    class Sampler
    {
        // The OpenGL object name of this sampler
        GLuint name;

    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name"
        Sampler()
        {
            // Create Sampler
            glGenSamplers(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler()
        {
            // Delete Sampler
            glDeleteSamplers(1, &name);
        }

        // This method binds this sampler to the given texture unit
        void bind(GLuint textureUnit) const
        {
            // Bind Sampler to texture unit
            glBindSampler(textureUnit, name);
        }

        // This static method ensures that no sampler is bound to the given texture unit
        static void unbind(GLuint textureUnit)
        {
            // Unbind Sampler from texture unit by binding it to 0
            glBindSampler(textureUnit, 0);
        }

        // This function sets a sampler parameter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        void set(GLenum parameter, GLint value) const
        {
            // set sampler integer parameter
            glSamplerParameteri(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        void set(GLenum parameter, GLfloat value) const
        {
            // set sampler float parameter
            glSamplerParameterf(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter
        void set(GLenum parameter, glm::vec4 value) const
        {
            // set sampler float vector parameter
            glSamplerParameterfv(name, parameter, &(value.r));
        }

        // Given a json object, this function deserializes the sampler state
        void deserialize(const nlohmann::json &data);

        Sampler(const Sampler &) = delete;
        Sampler &operator=(const Sampler &) = delete;
    };

}