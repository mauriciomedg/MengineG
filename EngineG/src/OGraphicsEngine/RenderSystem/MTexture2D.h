#pragma once
#include "../MPrerequisites.h"

namespace MG
{
	class MTexture2D
	{
	public:
		MTexture2D(const char* file_path);
		~MTexture2D();

		ui32 getId();

	private:
		ui32 m_textureID;
	};
}