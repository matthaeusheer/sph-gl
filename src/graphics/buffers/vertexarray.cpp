#include "vertexarray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ArrayID);
}

/*Delete all buffer objects from memory.*/
VertexArray::~VertexArray()
{
	for (int i = 0; i < m_Buffers.size(); i++)
		delete m_Buffers[i];
}

/*index ist the shader index (think of layout locataion = index)*/
void VertexArray::addBuffer(Buffer* buffer, GLuint index)
{
	bind();
	buffer->bind();

	m_Buffers.push_back(buffer);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);
	
	buffer->unbind();
	unbind();
}

void VertexArray::bind() const
{
	glBindVertexArray(m_ArrayID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}