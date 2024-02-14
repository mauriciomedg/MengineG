#include "MShaderProgram.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../Utils.h"
#include <iostream>

using namespace MG;

MShaderProgram::MShaderProgram(const MShaderProgramDesc& desc)
{
	m_programId = glCreateProgram();
	attach(desc.vertexShaderFilePath, MShaderType::VertexShader);
	attach(desc.fragmentShaderFilePath, MShaderType::FragmentShader);
	link();
}

void MShaderProgram::attach(const wchar_t* shaderFilePath, const MShaderType& type)
{
	std::string shaderCode = Utils::readShaderSource(shaderFilePath);

	ui32 shaderId = 0;
	if (type == MShaderType::VertexShader)
		shaderId = glCreateShader(GL_VERTEX_SHADER);
	else if (type == MShaderType::FragmentShader)
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);

	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	
	glCompileShader(shaderId);
	Utils::checkOpenGLError();
	GLint linked = 1;
	glGetProgramiv(m_programId, GL_COMPILE_STATUS, &linked);
	if (linked != 1) {
		std::cout << "Compile failed" << std::endl;
		Utils::printProgramLog(m_programId);
	}

	glAttachShader(m_programId, shaderId);
	m_attachedShaders[(ui32)type] = shaderId;
}

void MShaderProgram::link()
{
	GLint linked = 1;

	glLinkProgram(m_programId);
	Utils::checkOpenGLError();
	glGetProgramiv(m_programId, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		Utils::printProgramLog(m_programId);
	}
}

void MShaderProgram::setUniformBufferSlot(const char* name, ui32 slot)
{
	ui32 index = glGetUniformBlockIndex(m_programId, name);
	glUniformBlockBinding(m_programId, index, slot);
}

ui32 MShaderProgram::getId()
{
	return m_programId;
}

MShaderProgram::~MShaderProgram()
{
	for (ui32 shaderId : m_attachedShaders)
	{
		glDetachShader(m_programId, shaderId);
		glDeleteShader(shaderId);
	}

	glDeleteProgram(m_programId);
}