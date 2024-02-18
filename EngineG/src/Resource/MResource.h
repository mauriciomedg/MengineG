#pragma once

#include "../OGraphicsEngine/MPrerequisites.h"
#include <string>

namespace MG
{
	class MResource
	{
	public:
		MResource(const wchar_t* file_path, MResourceManager* resourceManager);
		virtual ~MResource();

	private:
		MResourceManager* m_resourceManager = nullptr;
		std::wstring m_full_path;
	};
}