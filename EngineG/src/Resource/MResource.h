#pragma once

#include "../OGraphicsEngine/MPrerequisites.h"
#include <string>

namespace MG
{
	class MResource
	{
	public:
		MResource(const char* file_path, MResourceManager* resourceManager);
		virtual ~MResource();

	private:
		MResourceManager* m_resourceManager = nullptr;
		std::string m_full_path;
	};
}