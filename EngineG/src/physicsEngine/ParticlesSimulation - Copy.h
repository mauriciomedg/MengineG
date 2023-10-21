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
	//int n; /* number of particles */
	float t = 0.0f; /* simulation clock */

	float* temp1 = nullptr;
	float* temp2 = nullptr;

	void init()
	{
		temp1 = new float[p.size() * 6];
		temp2 = new float[p.size() * 6];
	}
};

/* length of state derivative, and force vectors */
int ParticleDims(ParticleSystem* p)
{
	return(6 * p->p.size());
};

/* gather state from the particles into dst */
int ParticleGetState(ParticleSystem* p, float* dst)
{
	for (int i = 0; i < p->p.size(); i++)
	{
		*(dst++) = p->p[i]->x[0];
		*(dst++) = p->p[i]->x[1];
		*(dst++) = p->p[i]->x[2];
		*(dst++) = p->p[i]->v[0];
		*(dst++) = p->p[i]->v[1];
		*(dst++) = p->p[i]->v[2];
	}

	return 0;
}

/* scatter state from src into the particles */
int ParticleSetState(ParticleSystem* p, float* src) 
{
	for (int i = 0; i < p->p.size(); i++) {
		p->p[i]->x[0] = *(src++);
		p->p[i]->x[1] = *(src++);
		p->p[i]->x[2] = *(src++);
		p->p[i]->v[0] = *(src++);
		p->p[i]->v[1] = *(src++);
		p->p[i]->v[2] = *(src++);
	}
	
	return 0;
}

void Clear_Forces(ParticleSystem* p)
{
	for (int i = 0; i < p->p.size(); i++) {
		p->p[i]->f[0] = 0.0f;
		p->p[i]->f[1] = 0.0f;
		p->p[i]->f[2] = 0.0f;
	}
}

void Compute_Forces(ParticleSystem* p)
{
	// gravity and drag
	float k_drag = 0.01f;

	for (int i = 0; i < p->p.size(); i++) {
		p->p[i]->f[0] += -k_drag * p->p[i]->v[0];
		p->p[i]->f[1] += -9.8f * p->p[i]->m - k_drag * p->p[i]->v[0];
		p->p[i]->f[2] += - k_drag * p->p[i]->v[0];;
	}
}

///
bool Collision_Detection(Particle* p, float* Pos, float* N, float& depth)
{
	depth = 0.0f;

	for (int i = 0; i < 3; ++i)
	{
		depth += (p->x[i] - Pos[i]) * N[i];
	}
	
	return depth < 0.0f;
}

void Surface_Connection(Particle* p)
{
	float depth = 0.0f;
	float ks = 5;
	float kd = 1;
	float Pos[] = {0.0f, -1.0f, 0.0f};
	//float N[] = { 0.70710678f, 0.70710678f, 0.0f };
	float N[] = { 0.0f, 1.0f, 0.0f };

	if (Collision_Detection(p, Pos, N, depth))
	{
		p->f[0] += (-ks * depth - kd * p->v[0]) * N[0];
		p->f[1] += (-ks * depth - kd * p->v[1]) * N[1];
		p->f[2] += (-ks * depth - kd * p->v[2]) * N[2];
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
int ParticleDerivative(ParticleSystem* p, float* dst) {

	Clear_Forces(p); /* zero the force accumulators */
	Compute_Connections(p);
	Compute_Forces(p); /* magic force function */
	for (int i = 0; i < p->p.size(); i++) {
		*(dst++) = p->p[i]->v[0]; /* xdot = v */
		*(dst++) = p->p[i]->v[1];
		*(dst++) = p->p[i]->v[2];
		*(dst++) = p->p[i]->f[0] / p->p[i]->m; /* vdot = f/m */
		*(dst++) = p->p[i]->f[1] / p->p[i]->m;
		*(dst++) = p->p[i]->f[2] / p->p[i]->m;
	}

	return 0;
}

void ScaleVector(float* dst, int n, float DeltaT)
{
	for (int i = 0; i < n; i++) {
		*(dst++) *= DeltaT;
		*(dst++) *= DeltaT;
		*(dst++) *= DeltaT;
		*(dst++) *= DeltaT;
		*(dst++) *= DeltaT;
		*(dst++) *= DeltaT;
	}
}

void AddVectors(float* src, float* dst, int n)
{
	for (int i = 0; i < n; i++) {
		*(dst++) += *(src++);
		*(dst++) += *(src++);
		*(dst++) += *(src++);
		*(dst++) += *(src++);
		*(dst++) += *(src++);
		*(dst++) += *(src++);
	}
}

void EulerStep(ParticleSystem* p, float DeltaT)
{
	int nbSuperSample = 100;

	DeltaT = DeltaT / nbSuperSample;
	for (int sample = 0; sample < nbSuperSample; ++sample)
	{
		ParticleDerivative(p, p->temp1); /* get deriv */
		ScaleVector(p->temp1, p->p.size(), DeltaT); /* scale it */
		ParticleGetState(p, p->temp2); /* get state */
		AddVectors(p->temp1, p->temp2, p->p.size()); /* add -> temp2 */
		ParticleSetState(p, p->temp2); /* update state */
		p->t += DeltaT; /* update time */
	}
	
}