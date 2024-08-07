#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "MathLibrary/TwMat4.h"

class Utils
{
public:

	static GLuint loadTexture(const char* texImagePath);

	static std::string readShaderSource(const char* filePath);

	static GLuint createShaderProgram(const char* vp, const char* fp);

	static void printShaderLog(GLuint shader);

	static void printProgramLog(int prog);

	static bool checkOpenGLError();

	static glm::mat4 convertToglmmat4(const MG::TwMat4& mat);
};

class ImportedModel
{
private:
	int numVertices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normalVecs;
public:
	ImportedModel(const char* filePath);
	int getNumVertices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTextureCoords();
	std::vector<glm::vec3> getNormals();
};

class ModelImporter
{
private:
	// values as read in from OBJ file
	std::vector<float> vertVals;
	std::vector<float> stVals;
	std::vector<float> normVals;
	// values stored for later use as vertex attributes
	std::vector<float> triangleVerts;
	std::vector<float> textureCoords;
	std::vector<float> normals;
public:
	ModelImporter();
	void parseOBJ(const char* filePath);
	int getNumVertices();
	std::vector<float> getVertices();
	std::vector<float> getTextureCoordinates();
	std::vector<float> getNormals();
};

#endif // UTILS_H