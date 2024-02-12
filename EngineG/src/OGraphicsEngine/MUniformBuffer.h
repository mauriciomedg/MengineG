#pragma once

#include "MPrerequisites.h"

namespace MG
{
	class MUniformBuffer
	{
	public:
		MUniformBuffer(const MUniformBufferDesc& desc);
		~MUniformBuffer();

		void setData(void* data);
		ui32 getId();
	private:

		ui32 m_id = 0;
		ui32 m_size = 0;
	};
}