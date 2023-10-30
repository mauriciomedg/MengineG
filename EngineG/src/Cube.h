#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

class Cube
{
public:
	Cube();

	void init(GLuint* vbo);
	void update(float currentTime, Camera* camera, GLuint renderingProgram);
	glm::mat4& getModelMatrix() { return mMat; };
	
public:

	glm::mat4 mMat;

private:

	glm::vec3 mPos;
	glm::mat3 mR;

	GLuint* m_vbo;
};