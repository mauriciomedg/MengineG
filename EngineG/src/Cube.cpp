#include"Cube.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "physicsEngine/CollisionDetection.h"
#include "Utils.h"

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

void Cube::createCube(GLuint* vbo, GLuint* vboTextCoords)
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

	float pyrTextCoords[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, *vboTextCoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTextCoords), pyrTextCoords, GL_STATIC_DRAW);
}

void Cube::init(GLuint* vbo, GLuint* vboTextCoords, const glm::mat4& modelMat, const float halfSize)
{
	createCube(vbo, vboTextCoords);
	mBrickTexture = Utils::loadTexture("textures/block.png");
	mhalfSize = halfSize;
	setModelMatrix(modelMat);

	m_vbo = vbo;
	m_vbo_textureCoords = vboTextCoords;
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

	//for texture
	glBindBuffer(GL_ARRAY_BUFFER, *m_vbo_textureCoords);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mBrickTexture);

	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CW); // the cube vertices have clockwise winding order
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 number of vertices

}

