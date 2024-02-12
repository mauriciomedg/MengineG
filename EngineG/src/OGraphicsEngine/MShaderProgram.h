#pragma once

#include "MPrerequisites.h"

namespace MG
{
	class MShaderProgram
	{
	public:
		MShaderProgram(const MShaderProgramDesc& desc);
		~MShaderProgram();
		
		ui32 getId();
	private:

		void attach(const wchar_t* shaderFilePath, const MShaderType& type);
		void link();

	private:
		ui32 m_programId = 0;
		ui32 m_attachedShaders[2] = {};
	};
}