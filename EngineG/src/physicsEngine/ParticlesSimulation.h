#pragma once

#include <glm/gtc/type_ptr.hpp>
#include<vector>

struct Particle 
{
	float m; /* mass */
	glm::vec3 x; /* position vector */
	glm::vec3 v; /* velocity vector */
	glm::vec3 f; /* force accumulator */
	glm::vec3 f_input; /* force from inputs */
	glm::vec3 last_input_fetch;

	void addMovement(glm::vec3& intensity, float scale);

	glm::vec3 fetchInput();
	Particle();
};

struct ParticleSystem 
{
	glm::vec3 gravity = glm::vec3(0.0f, -98.0f, 0.0f);
	std::vector<Particle*> p; /* array of pointers to particles */
	float t = 0.0f; /* simulation clock */

	std::vector<glm::vec3> temp1;
	std::vector<glm::vec3> temp2;

	void init()
	{
		temp1.resize(p.size() * 2);
		temp2.resize(p.size() * 2);
	}
};

void EulerStep(ParticleSystem* p, float DeltaT);
