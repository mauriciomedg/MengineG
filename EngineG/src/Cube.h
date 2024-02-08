#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera;

class GShape
{
public:
	GShape();

	virtual void init(GLuint* vbo, GLuint* vboTextCoords, const glm::mat4& modelMat, const float halfSize) {};
	virtual void update(Camera* camera, GLuint renderingProgram) {};
	virtual void setModelMatrix(const glm::mat4& modelMat) {};

protected:
	glm::mat4 mMat;
	GLuint* m_vbo;
	GLuint* m_vbo_textureCoords;
};

class Cube : public GShape
{
	static void createCube(GLuint* vbo, GLuint* vboTextCoords);

public:
	Cube();
	virtual void init(GLuint* vbo, GLuint* vboTextCoords, const glm::mat4& modelMat, const float halfSize);
	virtual void update(Camera* camera, GLuint renderingProgram);
	virtual void setModelMatrix(const glm::mat4& modelMat);

private:

	float mhalfSize = 1.0f;
	//move to a manager resources.
	GLuint mBrickTexture;
};

class Sphere : public GShape
{
private:

	static float toRadians(float degrees);
public:
	//Sphere();
	//Sphere(int prec);
	//int getNumVertices();
	//int getNumIndices();
	//std::vector<int> getIndices();
	//std::vector<glm::vec3> getVertices();
	//std::vector<glm::vec2> getTexCoords();
	//std::vector<glm::vec3> getNormals();

	static void createSphere(int prec, GLuint* vbo, GLuint* vboTextCoords, int& numIndices);

public:
	
	virtual void init(GLuint* vbo, GLuint* vboTextCoords, const glm::mat4& modelMat, const float halfSize);
	virtual void update(Camera* camera, GLuint renderingProgram);
	virtual void setModelMatrix(const glm::mat4& modelMat);

private:
	GLuint mBrickTexture;
	int mNumIndices;
	float mRadius = 1.0f;
};

class Model
{
	GLuint mBrickTexture;
	glm::mat4 mMat;
	int mNumVertices;

	GLuint* m_vboVertex;
	GLuint* m_vboTextCoords;
	GLuint* m_vboNormals;

public:
	static void createModel(GLuint* vboVertex, GLuint* vboTextCoords, GLuint* vboNormals, int& numVertices);
	virtual void init(GLuint* vboVertex, GLuint* vboTextCoords, GLuint* vboNormals, const glm::mat4& modelMat);
	void update(Camera* camera, GLuint renderingProgram);
};


class Line {
    int shaderProgram;
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    glm::mat4 MVP;
    glm::vec3 lineColor;
public:
	Line(glm::vec3 start, glm::vec3 end);

    int setMVP(const glm::mat4& mvp);
    int setColor(const glm::vec3& color);
    int draw();

    ~Line();
};