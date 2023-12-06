#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

class Cube
{
public:
	Cube();

	void init(GLuint* vbo, const glm::mat4& modelMat);
	void update(Camera* camera, GLuint renderingProgram);
	glm::mat4& getModelMatrix() { return mMat; };
	void setModelMatrix(const glm::mat4& modelMat);
	
private:

	//void updateModelMatrix();

	glm::mat4 mMat;
	GLuint* m_vbo;
};