#pragma once

#include "MPrerequisites.h"

namespace MG
{
	class MVertexArrayObject
	{
	public:
		MVertexArrayObject(const MVertexBufferDesc& data);
		~MVertexArrayObject();

		ui32 getId();
		ui32 getVertexBufferSize();
		ui32 getVertexSize();

	private:
		ui32 m_vertexBufferObjectId = 0;
		ui32 m_vertexArrayObjectId = 0;
		MVertexBufferDesc m_vertexBufferData;
	};
}