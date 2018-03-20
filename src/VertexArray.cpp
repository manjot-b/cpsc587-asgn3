#include "VertexArray.h"
#include <iostream>

using namespace std;

VertexArray::VertexArray(
    const int componentsPerAttribute[], size_t vertCompSize, const float buffer[], size_t buffSize, GLenum _drawType)
: drawType(_drawType)
{
    glGenBuffers(1, &vbo); // gen buffer and store id in VBO
	glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &id);
    
    glBindVertexArray(id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,  buffSize * sizeof(float), buffer, drawType);

    const int totalComponents = sumArray(0, vertCompSize, componentsPerAttribute);
    const int stride = totalComponents * sizeof(float);
    for(unsigned int i = 0; i < vertCompSize; i++)
    {
        int components = componentsPerAttribute[i];
        int offset = sumArray(0, i, componentsPerAttribute);

        glVertexAttribPointer(i, components, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
	    glEnableVertexAttribArray(i);    
    }


	// unbind VAO, VBO, and EBO
	glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::~VertexArray()
{
    // cout << "DELETEING VERTEX ARRAY" << id << endl;
    glDeleteVertexArrays(1, &id);
	glDeleteBuffers(1, &vbo);
}

int VertexArray::sumArray(int start, int end, const int array[]) const
{
    int sum = 0;
    for (int i = start; i < end; i++)
    {
        sum += array[i];
    }
    return sum;
}

void VertexArray::use() const
{
    glBindVertexArray(id);
}

void VertexArray::unuse() const
{
    glBindVertexArray(0);
}

void VertexArray::updateBuffer(const float buffer[], size_t buffSize)
{
    glBindVertexArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffSize * sizeof(float), buffer);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::setElementBuffer(const unsigned int buffer[], size_t buffSize)
{
    glBindVertexArray(id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffSize * sizeof(unsigned int), buffer, drawType);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}













