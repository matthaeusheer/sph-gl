#pragma once

#include <vector>

#include "GLAD/glad.h"
#include "buffer.h"

class VertexArray
{
private:
	GLuint m_ArrayID;
	std::vector<Buffer*> m_Buffers;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(Buffer* buffer, GLuint index);
	void bind() const;
	void unbind() const;

};
