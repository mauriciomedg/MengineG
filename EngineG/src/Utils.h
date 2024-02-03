#pragma once

#include <GL/glew.h>
#include <string>

class Utils
{
public:

	static GLuint loadTexture(const char* texImagePath);

	static std::string readShaderSource(const char* filePath);

	static GLuint createShaderProgram(const char* vp, const char* fp);

	static void printShaderLog(GLuint shader);

	static void printProgramLog(int prog);

	static bool checkOpenGLError();
		
};