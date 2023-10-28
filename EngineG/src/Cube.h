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
	const glm::mat4& getModelMatrix() { return mMat; };
	
public:

	glm::vec3 mPos;
	glm::mat3 mR;
	
private:

	glm::mat4 mMat;
	GLuint* m_vbo;
};