#pragma once

#include <GLAD/glad.h>

class IndexBuffer
{
private:
	GLuint m_BufferID;
	GLuint m_Count; // what kind of vec, eg vec2, vec3

public:
	IndexBuffer(GLuint* data, GLsizei count);

	void bind() const;
	void unbind() const;

	GLuint getCount() const { return m_Count; }
};
