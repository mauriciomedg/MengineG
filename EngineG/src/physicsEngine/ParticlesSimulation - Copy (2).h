#pragma once

#include<vector>

struct Particle 
{
	float m = 0.5; /* mass */
	glm::vec3 x; /* position vector */
	glm::vec3 v; /* velocity vector */
	glm::vec3 f; /* force accumulator */
};

struct ParticleSystem 
{
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

/* length of state derivative, and force vectors */
int ParticleDims(ParticleSystem* p)
{
	return(p->p.size());
};

/* gather state from the particles into dst */
int ParticleGetState(ParticleSystem* p, std::vector<glm::vec3>& dst)
{
	for (int i = 0; i < p->p.size(); ++i)
	{
		dst[2 * i] = p->p[i]->x;
		dst[2 * i + 1] = p->p[i]->v;
	}

	return 0;
}

/* scatter state from src into the particles */
int ParticleSetState(ParticleSystem* p, std::vector<glm::vec3>& src)
{
	for (int i = 0; i < p->p.size(); ++i)
	{
		p->p[i]->x = src[2 * i];
		p->p[i]->v = src[2 * i + 1];
	}
	
	return 0;
}

void Clear_Forces(ParticleSystem* p)
{
	for (int i = 0; i < p->p.size(); i++) {
		p->p[i]->f = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void Compute_Forces(ParticleSystem* p)
{
	// gravity and drag
	float k_drag = 0.0001f;
	glm::vec3 gravity(0.0f, -9.8f, 0.0f);
	for (int i = 0; i < p->p.size(); i++) 
	{
		p->p[i]->f += -k_drag * p->p[i]->v + p->p[i]->m * gravity;
	}
}

///
bool Collision_Detection(Particle* p, glm::vec3& Pos, glm::vec3& N, float& depth)
{
	depth = glm::dot(p->x - Pos, N);
	return depth < 0.0f;
}

void Surface_Connection(Particle* p)
{
	float depth = 0.0f;
	float ks = 5;
	float kd = 1;
	glm::vec3 Pos(0.0f, -5.0f, 0.0f);
	//float N[] = { 0.70710678f, 0.70710678f, 0.0f };
	glm::vec3 N(0.0f, 1.0f, 0.0f);

	if (Collision_Detection(p, Pos, N, depth))
	{
		p->f += (-ks * depth - kd * glm::dot(p->v, N)) * N;
	}
}

void Compute_Connections(ParticleSystem* p)
{
	for (int i = 0; i < p->p.size(); i++)
	{
		Surface_Connection(p->p[i]);
	}
}

/* calculate derivative, place in dst */
int ParticleDerivative(ParticleSystem* p, std::vector<glm::vec3>& dst) {

	Clear_Forces(p); /* zero the force accumulators */
	Compute_Connections(p);
	Compute_Forces(p); /* magic force function */

	for (int i = 0; i < p->p.size(); ++i)
	{
		dst[2 * i] = p->p[i]->v;
		dst[2 * i + 1] = p->p[i]->f / p->p[i]->m;
	}

	return 0;
}

void ScaleVector(std::vector<glm::vec3>& dst, float DeltaT)
{
	for (int i = 0; i < dst.size(); ++i)
	{
		dst[i] *= DeltaT;
	}
}

void AddVectors(std::vector<glm::vec3>& src, std::vector<glm::vec3>& dst)
{
	for (int i = 0; i < src.size(); ++i) {
		dst[i] += src[i];
	}
}

void EulerStep(ParticleSystem* p, float DeltaT)
{
	int nbSuperSample = 2;

	DeltaT = DeltaT / nbSuperSample;
	for (int sample = 0; sample < nbSuperSample; ++sample)
	{
		ParticleDerivative(p, p->temp1); /* get deriv */
		ScaleVector(p->temp1, DeltaT); /* scale it */
		ParticleGetState(p, p->temp2); /* get state */
		AddVectors(p->temp1, p->temp2); /* add -> temp2 */
		ParticleSetState(p, p->temp2); /* update state */

		for (int i = 0; i < p->p.size(); ++i)
		{
			p->p[i]->x += p->p[i]->v * DeltaT;
		}
		
		p->t += DeltaT; /* update time */
	}
	
}