#pragma once

#include <glad/glad.h>
#include "src/utils/datamanager.h"

class Buffer
{
private:
	GLuint m_BufferID;
	GLuint m_ComponentCount; // what kind of vec, eg vec2, vec3

public:
	Buffer(GLfloat* data, GLsizei count, GLuint compononentCount);

	void Buffer::updateBuffer(size_t size, float* dataPointer);

	void bind() const;
	void unbind() const;

	GLuint getComponentCount() const { return m_ComponentCount; }
};