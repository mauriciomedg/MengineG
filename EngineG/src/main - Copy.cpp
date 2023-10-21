#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Utils.h"
#include "Cube.h"
#include "Camera.h"

#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[2];
GLuint mvLoc, projLoc;

using namespace std;

GLuint createShaderProgram() 
{
	string vertShaderStr = Utils::readShaderSource("shaders/vertShader.glsl");
	string fragShaderStr = Utils::readShaderSource("shaders/fragShader.glsl");

	const char* vshaderSource = vertShaderStr.c_str();
	const char* fshaderSource = fragShaderStr.c_str();
	
	return Utils::createShaderProgram(vshaderSource, fshaderSource);
}

float vPositions[] = { 0.25, -0.25, 0.0,
						-0.25, -0.25, 0.0,
						0.25, 0.25, 0.0 };

Cube oneCube;
Camera camera;

void init(GLFWwindow* window) 
{ 
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(2, vbo);
	
	oneCube.init(vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);
}

float x = 0.0f; // location of triangle on x axis
float v_i = 1.0f; // offset for moving the triangle
float v = v_i;
double lastTime = 0.0f;
double currentTime = 0;

void display(GLFWwindow* window, double currentTime)
{

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset"

	//////////////
	double current = glfwGetTime();
	double elapsed = current - lastTime;

	currentTime += elapsed;
	x += v * elapsed; // move the triangle along x axis
	if (x > 1.0f) v = -v_i; // switch to moving the triangle to the left
	if (x < -1.0f) v = v_i; // switch to moving the triangle to the right
	///////////////
	camera.update(window);
	oneCube.update(currentTime);

	glm::mat4 mvMat = camera.getViewMatrix() * oneCube.getModelMatrix();
	///////////////
	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
	glProgramUniform1f(renderingProgram, offsetLoc, x); // send value in "x" to "offset"

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // make the 0th buffer "active"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // associate 0th attribute with buffer
	glEnableVertexAttribArray(0); // enable the 0th vertex attribute

	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 number of vertices

	lastTime = glfwGetTime();
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}