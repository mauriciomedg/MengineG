#include"Pyramid.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

Pyramid::Pyramid()
	: mPos(5.0f, 1.0f, 0.0f)
{
}

void Pyramid::init(GLuint* vbo)
{
	m_vbo = &vbo[0];

	float vertexPositions[] = {
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
 -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
 -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base � left front
 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base � right back
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void Pyramid::update(float currentTime, Camera* camera, GLuint renderingProgram)
{
	glm::mat4 tMat = glm::translate(glm::mat4(1.0f),
		//glm::vec3(-sin(0.35f * currentTime) * 2.0f, -cos(0.52f * currentTime) * 2.0f, -sin(0.7f * currentTime) * 2.0f));
		mPos);
	glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	// the 1.75 adjusts the rotation speed
	mMat = tMat * rMat;

	glm::mat4 mvMat = camera->getViewMatrix() * getModelMatrix();

	GLuint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	GLuint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	//glProgramUniform1f(renderingProgram, offsetLoc, x); // send value in "x" to "offset"

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]); // make the 0th buffer "active"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associate 0th attribute with buffer
	glEnableVertexAttribArray(0); // enable the 0th vertex attribute

	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18); // 36 number of vertices
}