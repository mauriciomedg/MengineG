#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <map>

class RigidBody;

struct ContactData
{
	RigidBody* a;
	RigidBody* b;

	glm::vec3 p;
	glm::vec3 n;
};

class CollisionDetection
{
public:
	CollisionDetection();
	
	void getContact(RigidBody* b1, RigidBody* b2, std::vector<ContactData>& cData);

private:

	void addEdges(glm::vec4* dir);

	std::map<int, std::tuple<glm::vec4, glm::vec4>> mEdges;
};