#pragma once

#include <memory>

typedef float f32;
typedef int i32;
typedef unsigned int ui32;

namespace MG
{
	class MVertexArrayObject;
	class MShaderProgram;

	typedef std::shared_ptr<MVertexArrayObject> MVAOSharedPtr;
	typedef std::shared_ptr<MShaderProgram> MShaderProgSharedPtr;

	struct MVertexAtrribute
	{
		ui32 numElements = 0; // 2 -> vec2, 3 -> ve3, 4 -> vec4
	};

	struct MVertexBufferDesc
	{
		void* vertexList = nullptr;
		ui32 vertexSize = 0;
		ui32 listSize = 0;

		MVertexAtrribute* attributesList = nullptr;
		ui32 attributesListSize = 0;
	};

	struct MShaderProgramDesc
	{
		const wchar_t* vertexShaderFilePath;
		const wchar_t* fragmentShaderFilePath;
	};

	enum MShaderType
	{
		VertexShader = 0,
		FragmentShader
	};
}



