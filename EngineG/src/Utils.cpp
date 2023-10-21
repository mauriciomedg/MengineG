#include"Utils.h"
#include <iostream>
#include <fstream>

using namespace std;


string Utils::readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint Utils::createShaderProgram(const char* vp, const char* fp)
{
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	//const char* vshaderSource =
	//	"#version 430 \n"
	//	"void main(void) \n"
	//	"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";
	//const char* fshaderSource =
	//	"#version 430 \n"
	//	"out vec4 color; \n"
	//	"void main(void) \n"
	//	//"{ color = vec4(1.0, 1.0, 1.0, 1.0); }";
	//	"{ if (gl_FragCoord.x < 295) color = vec4(1.0, 0.0, 0.0, 1.0); else color = vec4(0.0, 0.0, 1.0, 1.0); }";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vp, NULL);
	glShaderSource(fShader, 1, &fp, NULL);

	// catch errors while compiling shaders
	glCompileShader(vShader);
	Utils::checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		Utils::printShaderLog(vShader);
	}
	glCompileShader(fShader);
	Utils::checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		Utils::printShaderLog(fShader);
	}
	///////////////////////////////////////
	// catch errors while linking shaders
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	Utils::checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		Utils::printProgramLog(vfProgram);
	}

	///////////////////////////////////////
	return vfProgram;
}

void Utils::printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}
void Utils::printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}
bool Utils::checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}