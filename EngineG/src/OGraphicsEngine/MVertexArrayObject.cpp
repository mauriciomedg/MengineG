#include "MVertexArrayObject.h"
#include <GL/glew.h>

using namespace MG;

MVertexArrayObject::MVertexArrayObject(const MVertexArrayBufferData& data)
{
	glGenBuffers(1, &m_vertexBufferObjectId);

	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, data.vertexSize * data.listSize, data.vertexList, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, data.vertexSize, 0); // associate 0th attribute with buffer
	glEnableVertexAttribArray(0); // enable the 0th vertex attribute


	glBindVertexArray(0);

	m_vertexBufferData = data;
}

MVertexArrayObject::~MVertexArrayObject()
{
	glDeleteBuffers(1, &m_vertexBufferObjectId);
	glDeleteVertexArrays(1, &m_vertexArrayObjectId);
}

ui32 MVertexArrayObject::getId()
{
	return m_vertexArrayObjectId;
}

ui32 MVertexArrayObject::getVertexBufferSize()
{
	return m_vertexBufferData.listSize;
}

ui32 MVertexArrayObject::getVertexSize()
{
	return m_vertexBufferData.vertexSize;
}

