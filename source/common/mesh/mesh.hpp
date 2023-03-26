#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        /**
         * @brief Construct a new Mesh object
         *
         * @param vertices which contain the vertex data.
         * @param elements which contain the indices of the vertices out of which each rectangle will be constructed.
         */
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            // // TODO: (Req 2) Write this function
            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            // 1. Create Vertex and Element buffers/arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            // 2. Bind Vertex Array Object
            glBindVertexArray(VAO);
            // 3. Copy our vertices array in a buffer for OpenGL to use
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
            // 4. Copy our index array in a element buffer for OpenGL to use
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), &elements[0], GL_STATIC_DRAW);
            // 5. Then set the vertex attributes pointers
            // Vertex Positions
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            // Vertex Normals
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            // Vertex Colors
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            // Vertex Texture Coords
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD); // 6. Unbind the VAO
            glBindVertexArray(0);
            // 7. Unbind the VBO
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // 8. Unbind the EBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            // 9. Remember the number of elements
            elementCount = elements.size();
        }

        // this function should render the mesh
        void draw()
        {
            // // TODO: (Req 2) Write this function
            // 1. Bind the VAO
            glBindVertexArray(VAO);
            // 2. Bind the EBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // 3. Draw the triangles
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
            // 4. Unbind the VAO
            glBindVertexArray(0);
            // 5. Unbind the EBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // // TODO: (Req 2) Write this function
            // 1. Delete the VAO
            glDeleteVertexArrays(1, &VAO);
            // 2. Delete the VBO
            glDeleteBuffers(1, &VBO);
            // 3. Delete the EBO
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };
}