#pragma once

#include "MPrerequisites.h"

namespace MG
{
	class MVertexArrayObject
	{
	public:
		MVertexArrayObject(const MVertexBufferDesc& vbDesc);
		MVertexArrayObject(const MVertexBufferDesc& vbDesc, const MIndexBufferDesc& ibDesc);
		~MVertexArrayObject();

		ui32 getId();
		ui32 getVertexBufferSize();
		ui32 getElementBufferSize();
		ui32 getVertexSize();

	private:
		ui32 m_vertexBufferObjectId = 0;
		ui32 m_elementBufferObjectId = 0;
		ui32 m_vertexArrayObjectId = 0;
		MVertexBufferDesc m_vertexBufferData;
		MIndexBufferDesc m_indexBufferData;
	};
}