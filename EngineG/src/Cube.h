#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

class GShape
{
public:
	GShape();

	virtual void init(GLuint* vbo, const glm::mat4& modelMat, const float halfSize) {};
	virtual void update(Camera* camera, GLuint renderingProgram) {};
	virtual void setModelMatrix(const glm::mat4& modelMat) {};

protected:
	glm::mat4 mMat;
	GLuint* m_vbo;
};

class Cube : public GShape
{
public:
	Cube();

	static void createCube(GLuint* vbo);
	virtual void init(GLuint* vbo, const glm::mat4& modelMat, const float halfSize);
	virtual void update(Camera* camera, GLuint renderingProgram);
	virtual void setModelMatrix(const glm::mat4& modelMat);

private:

	float mhalfSize = 1.0f;
};