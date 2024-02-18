#pragma once
#include "../OGraphicsEngine/MPrerequisites.h"
#include "MResource.h"

namespace MG
{
	class MMesh : public MResource
	{
	public:
		MMesh(const wchar_t* file_path, MResourceManager* resourceManager);
		MMesh(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes,
			MResourceManager* resourceManager);

		virtual ~MMesh();
	};
}