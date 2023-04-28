#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <map>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle (OpenGL object name)
        GLuint program;
        // a map to cache the uniform locations
        std::map<std::string, GLuint> uniform_location_cache;

    public:
        ShaderProgram()
        {
            // Create Shader Program
            program = glCreateProgram();
        }
        ~ShaderProgram()
        {
            // Delete Shader Program
            if (program != 0)
                glDeleteProgram(program);
            // reset the program to 0
            program = 0;
        }

        //  reads the shader source code from the given file send it to GPU, compile it then attach it to shader.
        bool attach(const std::string &filename, GLenum type) const;

        // Link Program after all shaders are attached
        bool link() const;

        void use()
        {
            // Use Shader Program
            glUseProgram(program);
        }

        /**
         * @brief Get the Uniform Location object in the shader
         *
         * @param name the name of the uniform variable
         * @return GLuint the location of the uniform variable
         */
        GLuint getUniformLocation(const std::string &name)
        {
            // we cache the uniform location to avoid calling glGetUniformLocation every time
            // we try to find the location of a uniform from the cache (if it exists)

            // create an iterator for cache map
            std::map<std::string, GLuint>::iterator it = uniform_location_cache.find(name);
            // if the uniform is found then return it
            if (it != uniform_location_cache.end())
            {
                // return the uniform location
                return it->second;
            }
            // else should create new uniform location and cache it then return it
            GLuint location = glGetUniformLocation(program, name.c_str());
            // cache the uniform location
            uniform_location_cache[name] = location;
            // return the uniform location
            return location;
        }

        /**
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, GLfloat value)
        {
            // set the value of the float uniform variable
            glUniform1f(getUniformLocation(uniform), value);
        }
        /**
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, GLuint value)
        {
            glUniform1ui(getUniformLocation(uniform), value);
        }
        /**
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, GLint value)
        {
            // set the value of the integer uniform variable
            glUniform1i(getUniformLocation(uniform), value);
        }
        /**
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, glm::vec2 value)
        {
            // set the value of the 2D vector uniform variable
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }
        /**
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, glm::vec3 value)
        {
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }
        /**
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, glm::vec4 value)
        {
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }
        /**
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param matrix: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, glm::mat4 matrix)
        {
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        // Question: Why do we delete the copy constructor and assignment operator?
        // Answer: since the class follows the RAII pattern, we should not allow copying the object
        // so if someone created an object and then copied it,
        // if one of them is destroyed,
        // the other one will still think that the program is valid which is not true
        // so we should delete the copy constructor and assignment operator
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
    };

}

#endif