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
            // // TODO: (Req 1) Create A shader program
            // Create Shader Program
            program = glCreateProgram();
        }
        ~ShaderProgram()
        {
            // // TODO: (Req 1) Delete a shader program
            // Delete Shader Program
            if (program != 0)
                glDeleteProgram(program);
            program = 0;
        }

        //  reads the shader source code from the given file send it to GPU, compile it then attach it to shader.
        bool attach(const std::string &filename, GLenum type) const;

        // Link Program after all shaders are attached
        bool link() const;

        void use()
        {
            glUseProgram(program);
        }

        // Get the location of a uniform variable in the shader
        GLuint getUniformLocation(const std::string &name)
        {
            // // TODO: (Req 1) Return the location of the uniform with the given name
            // we cache the uniform location to avoid calling glGetUniformLocation every time
            // we try to find the location of a uniform from the cache (if it exists)

            // create an iterator for cache map
            std::map<std::string, GLuint>::iterator it = uniform_location_cache.find(name);
            // if the uniform is found then return it
            if (it != uniform_location_cache.end())
            {
                return it->second;
            }
            // else should create new uniform location and cache it then return it
            GLuint location = glGetUniformLocation(program, name.c_str());
            uniform_location_cache[name] = location;
            return location;
        }

        /*
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, GLfloat value)
        {
            // // TODO: (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform), value);
        }
        /*
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, GLuint value)
        {
            // // TODO: (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform), value);
        }
        /*
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, GLint value)
        {
            // // TODO: (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform), value);
        }
        /*
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, glm::vec2 value)
        {
            // // TODO: (Req 1) Send the given 2D vector value to the given uniform
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }
        /*
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, glm::vec3 value)
        {
            // // TODO: (Req 1) Send the given 3D vector value to the given uniform
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }
        /*
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param value: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, glm::vec4 value)
        {
            // // TODO: (Req 1) Send the given 4D vector value to the given uniform
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }
        /*
         * Sets the value of a uniform variable in the shader.
         * @param uniform: the name of the uniform variable.
         * @param matrix: the value to set the uniform variable to.
         */
        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // // TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        // // TODO: (Req 1) Delete the copy constructor and assignment operator.
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