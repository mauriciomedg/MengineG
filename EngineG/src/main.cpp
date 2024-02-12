#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "Inputs.h"

#include "Utils.h"
#include "Camera.h"

#include "GEntity.h"
#include "physicsEngine/PhysicsWorld.h"
#include "Cube.h"

#include "MGame/MGame.h"

#define numVAOs 1
#define numVBOs 7
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mvLoc, projLoc;

////////////////////////////
PhysicsWorld* pWorld = new PhysicsWorld;

std::vector<GEntityBox*> gameObjectArray;
GEntityBoxControlled* boxControlled = new GEntityBoxControlled(pWorld);

////////////////////////////

using namespace std;

GLuint createShaderProgram() 
{
	string vertShaderStr = Utils::readShaderSource(L"shaders/vertShader.glsl");
	string fragShaderStr = Utils::readShaderSource(L"shaders/fragShader.glsl");

	const char* vshaderSource = vertShaderStr.c_str();
	const char* fshaderSource = fragShaderStr.c_str();
	
	return Utils::createShaderProgram(vshaderSource, fshaderSource);
}

Camera* camera = new Camera;

Sphere* sphere = new Sphere;

Model* model = new Model;

//Pyramid onePyramid;

void testBallSocket()
{
	std::vector<GEntityBox*> toConnect;
	for (int j = 0; j < 2; ++j)
	{
		GEntityBox* obj = new GEntityBox(pWorld);
		glm::vec3 Pos(20.0f, 45.0 - j * 5, 0.0f);
		auto R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
		obj->init(&vbo[0], &vbo[1], Mat, 3.0f, 1.0f, true);
		obj->setAffectedByGravity(true);
		gameObjectArray.push_back(obj);
		toConnect.push_back(obj);
	}

	pWorld->addRigidPointConstraint(toConnect[0]->getPhysicsID(), glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(20.0f, 65.0f, 0.0f), 10.0f);
	pWorld->addRigidPointRigidPointConstraint(toConnect[0]->getPhysicsID(), glm::vec3(-3.0f, -3.0f, -3.0f), toConnect[1]->getPhysicsID(), glm::vec3(3.0f, 3.0f, 3.0f), 15.0f);

}

void init(GLFWwindow* window) 
{
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	// Camera
	glm::vec3 Pos = glm::vec3(0.0f, 21.0f, 60.0f);
	//auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	auto Mat = glm::translate(glm::mat4(1.0f), Pos);
	camera->init(window, Mat, pWorld, false);

	// box controlled
	Pos = glm::vec3(-10.0f, 30.0f, 0.0f);
	auto R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
	boxControlled->init(&vbo[0], &vbo[1], Mat, 5.0f, 0.8f, true);
	
	for (int j = 0; j < 1; ++j)
	{
		GEntityBox* obj = new GEntityBox(pWorld);
		glm::vec3 Pos(50.0f, 25.0, 0.0f);
		auto R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
		obj->init(&vbo[0], &vbo[1], Mat, 10.0f, 1.0f, true);
		obj->setAffectedByGravity(true);
		gameObjectArray.push_back(obj);
	
	}
	
	testBallSocket();
	
	// Sphere
	Pos = glm::vec3(-5.0f, 30.0f, 0.0f);
	Mat = glm::translate(glm::mat4(1.0f), Pos);
	sphere->init(&vbo[2], &vbo[3], Mat, 10.0f);
	// 
	
	// Model
	Pos = glm::vec3(0.0f, 25.0, 0.0f);
	Mat = glm::translate(glm::mat4(1.0f), Pos);
	model->init(&vbo[4], &vbo[5], &vbo[6], Mat);
	// Plane
	Pos = glm::vec3(0.0f, -5.0f, 0.0f);
	//R = glm::mat4(1.0f);
	R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
	Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
	pWorld->instanciatePrimitivePlane(Mat, 1.0, false);
	
	pWorld->init();
}

double lastTime = 0.0f;
double currentTime = 0;

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
	glEnable(GL_CULL_FACE);
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	//GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset"
	
	//////////////
	double current = glfwGetTime();
	double elapsed = current - lastTime;
	currentTime += elapsed;

	//
	Inputs::get().update(window);
	//
	pWorld->simulating(elapsed);
	camera->update();

	for (GEntityBox* obj : gameObjectArray)
	{
		obj->update(camera, renderingProgram);
	}

	boxControlled->update(camera, renderingProgram);
	///

	//sphere->update(camera, renderingProgram);
	model->update(camera, renderingProgram);

	lastTime = glfwGetTime();

	//Utils::drawLine(glm::vec3(0.0f), glm::vec3(10.0f), 10.0f);

	//GLfloat line[] = {
	//	10.0f, 10.0f, 10.0f,
	//	0.0f, 0.0f, 0.0f
	//};
	//
	//glEnable(GL_LINE_SMOOTH);
	//glLineWidth(10);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, line);
	//glDrawArrays(GL_LINES, 0, 2);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisable(GL_LINE_SMOOTH);
}

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	camera->reshapeWindow(window, newWidth, newHeight);
}



int main(void) 
{
	MG::MGame game;
	
	game.init();
	game.update();

	//C++ OpenGL 3D Game Tutorial 2: Making OpenGL 3D Engine

	//if (!glfwInit()) { exit(EXIT_FAILURE); }
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
	//glfwMakeContextCurrent(window);
	//
	//if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	//glfwSwapInterval(1);
	//init(window);
	//glfwSetWindowSizeCallback(window, window_reshape_callback);
	//
	//Inputs& inputs = Inputs::get();
	//inputs.mapInput("MoveForward", "up", 1.0f);
	//inputs.mapInput("MoveLeft", "left", -1.0f);
	//inputs.mapInput("MoveRight", "right", 1.0f);
	//inputs.mapInput("MoveForwardCamera", "w", 1.0f);
	//inputs.mapInput("MoveBackwardCamera", "s", -1.0f);
	//inputs.mapInput("MoveLeftCamera", "a", -1.0f);
	//inputs.mapInput("MoveRightCamera", "d", 1.0f);
	//
	//while (!glfwWindowShouldClose(window)) {
	//	display(window, glfwGetTime());
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();	
	//}
	//glfwDestroyWindow(window);
	//glfwTerminate();
	//exit(EXIT_SUCCESS);
}