#pragma once
/*
*   A class which represents a Vertex Array Object
*/

#include <glad/glad.h>


class VertexArray
{
    public:
    
        /*
           parameters:
                vertexComponents:   The number of components for each type of data for the vertex 
                                    should be at every element. If the first 3 elements in buffer are the vertex
                                    positions, the next 2 are texture coordinates, and the last 3 are RGB colors 
                                    for the vertex then vertexComponents = {3, 2, 3}
                vertCompSize:       Size of vertexComponents
                buffer:             The buffer that holds all the vertex data, eg position, color, texture coords etc...
                buffSize:           The size of the buffer
        */
        VertexArray() {};
        VertexArray(
            const int componentsPerAttribute[], size_t vertCompSize, const float buffer[], size_t buffSize, GLenum drawUsage = GL_STATIC_DRAW);
        ~VertexArray();
        GLuint getID() const { return id; };

        void use() const;
        void unuse() const;
        void updateBuffer(const float buffer[], size_t buffSize);
    private:
        GLuint id;
        GLuint vbo;
        int sumArray(int start, int end, const int array[]) const;
};
