#pragma once

#include <vector>

typedef void (*dydt_func)(float t, float y[], float ydot[]);

class PhysicsWorld
{
private:

	int m_numberOfNodes;
	std::vector<float> m_stateVector;

	void ode(float y0[], float yEnd[], int len, float t0, float t1, dydt_func dydt);

};