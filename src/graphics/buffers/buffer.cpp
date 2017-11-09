#include "buffer.h"
#include "src/graphics/window.h"
#include "src/utils/datamanager.h"

Buffer::Buffer(GLfloat* data, GLsizei count, GLuint componentCount)
	: m_ComponentCount(componentCount)
{
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::updateBuffer(size_t size, float* dataPointer)
{
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), dataPointer);
	unbind();
}

void Buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
}
void Buffer::unbind() const 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}