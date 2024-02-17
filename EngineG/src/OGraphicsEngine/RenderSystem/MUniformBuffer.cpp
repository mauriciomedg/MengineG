#include "MUniformBuffer.h"
#include <GL/glew.h>

using namespace MG;

MUniformBuffer::MUniformBuffer(const MUniformBufferDesc& desc)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferData(GL_UNIFORM_BUFFER, desc.size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_size = desc.size;
}

void MUniformBuffer::setData(void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

ui32 MUniformBuffer::getId()
{
	return m_id;
}

MUniformBuffer::~MUniformBuffer()
{
	glDeleteBuffers(1, &m_id);
}


