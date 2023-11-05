#include "CollisionDetection.h"
#include "RigidBody.h"


namespace
{
	float intersectLinePlane(const glm::vec3& L1, const glm::vec3& L2, const glm::vec3& N, const glm::vec3& P)
	{
		auto d1 = L1 - P;
		auto d2 = L2 - P;

		if (glm::dot(d1, d2) <= 0)
		{
			return 0.f; //d1.length();
		}

		return -1.0;
	}
}

void CollisionDetection::addEdges(glm::vec4* dir)
{
	int size = mEdges.size();
	for (int i = 0; i < 3; ++i)
	{
		auto pair = mEdges[i + size - 1];
		mEdges[i + size] = std::make_tuple(std::get<0>(pair) + 2.0f * dir[i], std::get<1>(pair) + 2.0f * dir[i]);
	}
}

CollisionDetection::CollisionDetection()
{
	glm::vec4 dir[] = { glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),
						glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f),
						glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
						glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)
	};

	mEdges[0] = std::make_tuple(glm::vec4(1.0f), glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
	
	addEdges(dir);

	mEdges[mEdges.size()] = std::make_tuple(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f));

	glm::vec4 dir2[] = { glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),
						glm::vec4(0.0f, 0.0f, -1.0f, 0.0f),
						glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
						glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)
	};

	addEdges(dir2);
}

void CollisionDetection::getContact(RigidBody* b1, RigidBody* b2, std::vector<ContactData>& cData)
{
	glm::vec4 vertex[] = { 
			glm::vec4(1.0f),
			glm::vec4(-1.0f, -1.0f, -1.0f, 1.0),
			glm::vec4(-1.0f, 1.0f, -1.0f, 1.0),
			glm::vec4(1.0f, -1.0f, 1.0f, 1.0),
			glm::vec4(1.0f, -1.0f, -1.0f, 1.0),
			glm::vec4(-1.0f, -1.0f, 1.0f, 1.0),
			glm::vec4(-1.0f, 1.0f, 1.0f, 1.0),
			glm::vec4(1.0f, 1.0f, -1.0f, 1.0)
	};
	
}
