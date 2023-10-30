#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

class Cube
{
public:
	Cube();

	void init(GLuint* vbo, const glm::mat4& modelMat);
	void update(float currentTime, Camera* camera, GLuint renderingProgram);
	glm::mat4& getModelMatrix() { return mMat; };
	void setModelMatrix(const glm::mat4& modelMat);
	
private:

	//void updateModelMatrix();

	glm::mat4 mMat;
	//glm::vec3 mPos;
	//glm::mat3 mR;

	GLuint* m_vbo;
};