#pragma once

#include <memory>

typedef float f32;
typedef int i32;
typedef unsigned int ui32;

namespace MG
{
	class MVertexArrayObject;
	typedef std::shared_ptr<MVertexArrayObject> MVAOSharedPtr;

	struct MVertexArrayBufferData
	{
		void* vertexList = nullptr;
		ui32 vertexSize = 0;
		ui32 listSize = 0;
	};
}



