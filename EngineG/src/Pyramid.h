#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

class Pyramid
{
public:
	Pyramid();

	void init(GLuint* vbo);
	void update(float currentTime, Camera* camera, GLuint renderingProgram);
	const glm::mat4& getModelMatrix() { return mMat; };

	glm::vec3 mPos;

private:
	
	glm::mat4 mMat;
	GLuint* m_vbo;
};