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

#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[2];
GLuint mvLoc, projLoc;

////////////////////////////
PhysicsWorld* pWorld = new PhysicsWorld;

std::vector<GEntityBox*> gameObjectArray;
GEntityBoxControlled* boxControlled = new GEntityBoxControlled(pWorld);

////////////////////////////

using namespace std;

GLuint createShaderProgram() 
{
	string vertShaderStr = Utils::readShaderSource("shaders/vertShader.glsl");
	string fragShaderStr = Utils::readShaderSource("shaders/fragShader.glsl");

	const char* vshaderSource = vertShaderStr.c_str();
	const char* fshaderSource = fragShaderStr.c_str();
	
	return Utils::createShaderProgram(vshaderSource, fshaderSource);
}

Camera* camera = new Camera;

//Pyramid onePyramid;

void init(GLFWwindow* window) 
{
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(2, vbo);
	
	// Camera
	glm::vec3 Pos = glm::vec3(0.0f, 21.0f, 60.0f);
	//auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	auto Mat = glm::translate(glm::mat4(1.0f), Pos);
	camera->init(Mat, pWorld, false);

	// box controlled
	Pos = glm::vec3(10.0f, 40.0f, 0.0f);
	auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
	boxControlled->init(&vbo[1], Mat, 3.0f, 3.0f, true);
	
	//pWorld->addRigidPointConstraint(boxControlled->getPhysicsID());
	//
	{
		GEntityBox* obj = new GEntityBox(pWorld);
		glm::vec3 Pos(10.0f, 15.0, 0.0f);
		auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::vec3(1.0f, 0.0f, 0.0f));
		auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
		obj->init(&vbo[0], Mat, 5.0f, 1.0f, true);
		obj->setAffectedByGravity(true);
		gameObjectArray.push_back(obj);

		pWorld->addRigidPointRigidPointConstraint(obj->getPhysicsID(), glm::vec3(-5.0f, -5.0f, -5.0f), boxControlled->getPhysicsID(), glm::vec3(3.0f, 3.0f, 3.0f), 10.0f);
		pWorld->addRigidPointConstraint(obj->getPhysicsID(), glm::vec3(3.0f, 3.0f, 3.0f), 10.0f);
	}
	
	//for (int h = 0; h < 10; ++h)
	//{
	//	for (int j = 0; j < 3; ++j)
	//	{
	//		GEntityBox* obj = new GEntityBox(pWorld);
	//		glm::vec3 Pos(-5.0f + j * 10.0f, 20 + h * 5.0f, 25.0f);
	//		auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::vec3(1.0f, 0.0f, 0.0f));
	//		auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
	//		obj->setHalfSize(2.0f);
	//		obj->init(&vbo[0], Mat, true);
	//		obj->setAffectedByGravity(true);
	//		gameObjectArray.push_back(obj);
	//	}
	//}

	// Plane
	Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	R = glm::mat4(1.0f);
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
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset"
	
	//////////////
	double current = glfwGetTime();
	double elapsed = current - lastTime;
	currentTime += elapsed;

	//
	Inputs::get().update(window);
	//
	pWorld->simulating(elapsed);// runSimulation(elapsed);
	camera->update(window);

	for (GEntityBox* obj : gameObjectArray)
	{
		obj->update(camera, renderingProgram);
	}

	boxControlled->update(camera, renderingProgram);
	///
	
	lastTime = glfwGetTime();
}

int main(void) 
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);
	
	Inputs& inputs = Inputs::get();
	inputs.mapInput("MoveForward", "up", 1.0f);
	inputs.mapInput("MoveLeft", "left", -1.0f);
	inputs.mapInput("MoveRight", "right", 1.0f);
	inputs.mapInput("MoveForwardCamera", "w", 1.0f);
	inputs.mapInput("MoveBackwardCamera", "s", -1.0f);
	inputs.mapInput("MoveLeftCamera", "a", -1.0f);
	inputs.mapInput("MoveRightCamera", "d", 1.0f);
	
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();	
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}