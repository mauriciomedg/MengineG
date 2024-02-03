#include"Cube.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "physicsEngine/CollisionDetection.h"

GShape::GShape()
	: mMat(1.0f), m_vbo(nullptr)
{
}

Cube::Cube()
{
}

void Cube::setModelMatrix(const glm::mat4& modelMat)
{
	mMat = modelMat;
}

void Cube::createCube(GLuint* vbo)
{
	float vertexPositions[] = {
		-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void Cube::init(GLuint* vbo, const glm::mat4& modelMat, const float halfSize)
{
	createCube(vbo);
	mhalfSize = halfSize;
	setModelMatrix(modelMat);

	m_vbo = vbo;
}

void Cube::update(Camera* camera, GLuint renderingProgram)
{
	mMat *= glm::scale(glm::mat4(1.0f), glm::vec3(mhalfSize, mhalfSize, mhalfSize));

	glm::mat4 mvMat = camera->getViewMatrix() * mMat;
	
	GLuint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	GLuint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	GLuint halfSizeLoc = glGetUniformLocation(renderingProgram, "halfSize"); // get ptr to "offset"
	//glUniform1f(halfSizeLoc, mhalfSize); // send value in "x" to "offset"

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, *m_vbo); // make the 0th buffer "active"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associate 0th attribute with buffer
	glEnableVertexAttribArray(0); // enable the 0th vertex attribute

	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 number of vertices

}

