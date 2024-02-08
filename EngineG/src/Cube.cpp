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
	auto scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(mhalfSize, mhalfSize, mhalfSize));
	glm::mat4 mvMat = camera->getViewMatrix() * mMat * scaleMat;
	
	GLuint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	GLuint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	//GLuint halfSizeLoc = glGetUniformLocation(renderingProgram, "halfSize"); // get ptr to "offset"
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
//////////////////////////////////////////////////////////////////////
//Sphere::Sphere() {
//    init(48);
//}
//Sphere::Sphere(int prec) { // prec is precision, or number of slices
//    init(prec);
//}

float Sphere::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Sphere::createSphere(int prec, GLuint* vbo, GLuint* vboTextCoords, int& numIndices)
{
    int numVertices;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); } // std::vector::push_back()
    for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); } // inserts new element at
    for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); } // the end of a vector and
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); } // increases the vector size by 1
    // calculate triangle vertices
    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
            float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
            vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
            texCoords[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
            normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
        }
    }
    // calculate triangle indices
    for (int i = 0; i < prec; i++) {
        for (int j = 0; j < prec; j++) {
            indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
            indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
            indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors

    for (int i = 0; i < numIndices; i++) {
        pvalues.push_back((vertices[indices[i]]).x);
        pvalues.push_back((vertices[indices[i]]).y);
        pvalues.push_back((vertices[indices[i]]).z);
        tvalues.push_back((texCoords[indices[i]]).s);
        tvalues.push_back((texCoords[indices[i]]).t);
        nvalues.push_back((normals[indices[i]]).x);
        nvalues.push_back((normals[indices[i]]).y);
        nvalues.push_back((normals[indices[i]]).z);
    }

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, *vboTextCoords);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
}

void Sphere::setModelMatrix(const glm::mat4& modelMat)
{
    mMat = modelMat;
}

void Sphere::init(GLuint* vbo, GLuint* vboTextCoords, const glm::mat4& modelMat, const float halfSize)
{
    createSphere(48, vbo, vboTextCoords, mNumIndices);

    setModelMatrix(modelMat);
    mBrickTexture = Utils::loadTexture("textures/block.png");
    m_vbo = vbo;
    m_vbo_textureCoords = vboTextCoords;
    mRadius = halfSize;
}

void Sphere::update(Camera* camera, GLuint renderingProgram)
{
    float s = mRadius / glm::sqrt(3);
    auto scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(s, s, s));
    glm::mat4 mvMat = camera->getViewMatrix() * mMat * scaleMat;

    GLuint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    GLuint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    // copy perspective and MV matrices to corresponding uniform variables
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
    //GLuint halfSizeLoc = glGetUniformLocation(renderingProgram, "halfSize"); // get ptr to "offset"
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
    glFrontFace(GL_CCW); // the cube vertices have clockwise winding order
    glDrawArrays(GL_TRIANGLES, 0, mNumIndices); // 36 number of vertices
}

// accessors
//int Sphere::getNumVertices() { return numVertices; }
//int Sphere::getNumIndices() { return numIndices; }
//std::vector<int> Sphere::getIndices() { return indices; }
//std::vector<glm::vec3> Sphere::getVertices() { return vertices; }
//std::vector<glm::vec2> Sphere::getTexCoords() { return texCoords; }
//std::vector<glm::vec3> Sphere::getNormals() { return normals; }

///////////////////////////////////////////////////////////////////////

void Model::createModel(GLuint* vboVertex, GLuint* vboTextCoords, GLuint* vboNormals, int& numVertices)
{
    ImportedModel myModel("models/BlockModel3.obj");

    std::vector<glm::vec3> vert = myModel.getVertices();
    std::vector<glm::vec2> tex = myModel.getTextureCoords();
    std::vector<glm::vec3> norm = myModel.getNormals();
    int numObjVertices = myModel.getNumVertices();
    numVertices = numObjVertices;
    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors
    for (int i = 0; i < numObjVertices; i++) {
        pvalues.push_back((vert[i]).x);
        pvalues.push_back((vert[i]).y);
        pvalues.push_back((vert[i]).z);
        tvalues.push_back((tex[i]).s);
        tvalues.push_back((tex[i]).t);
        nvalues.push_back((norm[i]).x);
        nvalues.push_back((norm[i]).y);
        nvalues.push_back((norm[i]).z);
    }
 
    // VBO for vertex locations
    glBindBuffer(GL_ARRAY_BUFFER, *vboVertex);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
    // VBO for texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, *vboTextCoords);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
    // VBO for normal vectors
    glBindBuffer(GL_ARRAY_BUFFER, *vboNormals);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

}
    
void Model::init(GLuint* vboVertex, GLuint* vboTextCoords, GLuint* vboNormals, const glm::mat4& modelMat)
{
    createModel(vboVertex, vboTextCoords, vboNormals, mNumVertices);
    mBrickTexture = Utils::loadTexture("textures/block.png");
    mMat = modelMat;

    m_vboVertex     = vboVertex;
    m_vboTextCoords = vboTextCoords;
    m_vboNormals    = vboNormals;
}
    
void Model::update(Camera* camera, GLuint renderingProgram)
{
    float s = 1.0f;
    auto scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(s, s, s));
    glm::mat4 mvMat = camera->getViewMatrix() * mMat * scaleMat;

    GLuint mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    GLuint projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    // copy perspective and MV matrices to corresponding uniform variables
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
    //GLuint halfSizeLoc = glGetUniformLocation(renderingProgram, "halfSize"); // get ptr to "offset"
    //glUniform1f(halfSizeLoc, mhalfSize); // send value in "x" to "offset"

    // associate VBO with the corresponding vertex attribute in the vertex shader
    glBindBuffer(GL_ARRAY_BUFFER, *m_vboVertex); // make the 0th buffer "active"
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associate 0th attribute with buffer
    glEnableVertexAttribArray(0); // enable the 0th vertex attribute

    //for texture
    glBindBuffer(GL_ARRAY_BUFFER, *m_vboTextCoords);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    //for normals
    glBindBuffer(GL_ARRAY_BUFFER, *m_vboNormals);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mBrickTexture);

    // adjust OpenGL settings and draw model
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CCW); // the cube vertices have clockwise winding order
    glDrawArrays(GL_TRIANGLES, 0, mNumVertices); // 36 number of vertices
}


///////////////////////////////////////////////////////////////////////
Line::Line(glm::vec3 start, glm::vec3 end) {

    startPoint = start;
    endPoint = end;
    lineColor = glm::vec3(1, 1, 1);
    MVP = glm::mat4(1.0f);

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 MVP;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(color, 1.0f);\n"
        "}\n\0";

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    vertices.assign({
         start.x, start.y, start.z,
         end.x, end.y, end.z,
    
    });
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

int Line::setMVP(const glm::mat4& mvp) {
    MVP = mvp;
    return 1;
}

int Line::setColor(const glm::vec3& color) {
    lineColor = color;
    return 1;
}

int Line::draw() {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    return 1;
}

Line::~Line() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

