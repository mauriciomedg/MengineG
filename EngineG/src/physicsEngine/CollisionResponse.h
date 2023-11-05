#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class RigidBody;

class Contact
{

public:
  
    RigidBody* body[2];

    glm::vec3 contactPoint;
    glm::vec3 contactNormal;
    float contactDepth;
    int localContactId;

    Contact* nextContact;
};

struct ContactGenRegistration
{
    Contact* gen;
    ContactGenRegistration* next;
};

class CollisionResponse
{
    enum {
        MaxContacts = 100
    };

    ContactGenRegistration* mFirstContact;

    Contact* mContacts;
    Contact* mContactItt;
public:

    typedef void (*dydt_func)(float* y, float* ydot, float deltaT, int size);

    CollisionResponse();
	void resolverContacts(std::vector<RigidBody*>& bodies, float* y, float* ydot, float dt, dydt_func solver);
    void generateContacts(std::vector<RigidBody*>& bodies, float dt);
};