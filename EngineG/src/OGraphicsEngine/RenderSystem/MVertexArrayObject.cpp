#include "MVertexArrayObject.h"
#include <GL/glew.h>

using namespace MG;

MVertexArrayObject::MVertexArrayObject(const MVertexBufferDesc& vbDesc)
{
	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId);

	glGenBuffers(1, &m_vertexBufferObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, vbDesc.vertexSize * vbDesc.listSize, vbDesc.vertexList, GL_STATIC_DRAW);

	for (ui32 i = 0; i < vbDesc.attributesListSize; ++i)
	{
		glVertexAttribPointer(
			i, //0,
			vbDesc.attributesList[i].numElements,//3,
			GL_FLOAT,
			GL_FALSE,
			vbDesc.vertexSize,
			(void*)((i == 0) ? 0 : vbDesc.attributesList[i - 1].numElements * sizeof(f32)) // offset in bytes from the previous attribute.
		);

		glEnableVertexAttribArray(i); // enable the ith vertex attribute
	}
	
	glBindVertexArray(0);

	m_vertexBufferData = vbDesc;
}

MVertexArrayObject::MVertexArrayObject(const MVertexBufferDesc& vbDesc, const MIndexBufferDesc& ibDesc)
	: MVertexArrayObject(vbDesc)
{
	glBindVertexArray(m_vertexArrayObjectId);

	glGenBuffers(1, &m_elementBufferObjectId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObjectId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibDesc.listSize, ibDesc.indicesList, GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_indexBufferData = ibDesc;
}

MVertexArrayObject::~MVertexArrayObject()
{
	glDeleteBuffers(1, &m_elementBufferObjectId);
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

ui32 MVertexArrayObject::getElementBufferSize()
{
	return m_indexBufferData.listSize;
}

ui32 MVertexArrayObject::getVertexSize()
{
	return m_vertexBufferData.vertexSize;
}

