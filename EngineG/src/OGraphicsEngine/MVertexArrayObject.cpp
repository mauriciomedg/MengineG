#include "MVertexArrayObject.h"
#include <GL/glew.h>

using namespace MG;

MVertexArrayObject::MVertexArrayObject(const MVertexBufferDesc& data)
{
	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId);

	glGenBuffers(1, &m_vertexBufferObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, data.vertexSize * data.listSize, data.vertexList, GL_STATIC_DRAW);

	for (ui32 i = 0; i < data.attributesListSize; ++i)
	{
		glVertexAttribPointer(
			i, //0,
			data.attributesList[i].numElements,//3,
			GL_FLOAT,
			GL_FALSE,
			data.vertexSize,
			(void*)((i == 0) ? 0 : data.attributesList[i - 1].numElements * sizeof(f32)) // offset in bytes from the previous attribute.
		);

		glEnableVertexAttribArray(i); // enable the ith vertex attribute
	}
	
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

