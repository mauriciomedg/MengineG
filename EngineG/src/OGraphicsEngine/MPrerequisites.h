#pragma once

#include <memory>
#include <glm/glm.hpp>

typedef float f32;
typedef int i32;
typedef unsigned int ui32;

namespace MG
{
	class MVertexArrayObject;
	class MUniformBuffer;
	class MShaderProgram;
	class MDeviceContext;
	class MRenderSystem;
	class MResource;
	class MResourceManager;

	typedef std::shared_ptr<MVertexArrayObject> MVAOSharedPtr;
	typedef std::shared_ptr<MUniformBuffer> MUniformBufferSharedPtr;
	typedef std::shared_ptr<MShaderProgram> MShaderProgSharedPtr;
	typedef std::shared_ptr<MDeviceContext> MDeviceContextSharedPtr;
	typedef std::shared_ptr<MResource> MResourceSharedPtr;
	

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

	struct MIndexBufferDesc
	{
		void* indicesList = nullptr;
		ui32 listSize = 0;
	};

	struct MShaderProgramDesc
	{
		const wchar_t* vertexShaderFilePath;
		const wchar_t* fragmentShaderFilePath;
	};

	struct MUniformBufferDesc
	{
		ui32 size = 0;
	};

	struct VertexMesh
	{
		glm::vec3 position;
		glm::vec2 textcoord;
	};

	enum class MTriangleType
	{
		TriangleList = 0,
		TriangleStrip
	};

	enum class MCullType
	{
		BackFace = 0,
		FrontFace,
		Both
	};

	enum class MWindingOrder
	{
		ClockWise = 0,
		CounterClockWise
	};

	enum class MShaderType
	{
		VertexShader = 0,
		FragmentShader
	};
}



