#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class RigidBody;

class Contact
{

public:
  
    enum {
        RESTING = 0,
        MOVING_AWAY = 1,
        COLLIDING = 2
    };

    Contact();

    int type;
    RigidBody* body[2];

    glm::vec3 contactPoint;
    glm::vec3 contactNormal;
    glm::vec3 relContactPoint;
    glm::vec3 contactVelocity;

    float contactDepth;
    int localContactId;

    bool isRestingContact = false;
    void applyPositionChange(glm::vec3 linearChange[2],
        glm::vec3 angularChange[2],
        float penetration,
        float dt);

    void applyVelocityChange(glm::vec3 velocityChange[2],
        glm::vec3 rotationChange[2]);

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
    void adjustPositions(std::vector<RigidBody*>& bodies, Contact* c, int nbContacts, float dt);
    void adjustVelocities(std::vector<RigidBody*>& bodies, Contact* c, int nbContacts, float dt);
    void resolverContacts(std::vector<RigidBody*>& bodies, float* y, float* ydot, float dt, dydt_func solver);
    void resolve(RigidBody* bodies, glm::vec4 pLocal, float* y, float* ydot, float dt0, float dt1, float dtm, dydt_func solver);
    void resolve2(RigidBody* bodies, glm::vec4 pLocal, float* y, float* ydot, float dt0, float dt1, float dtm, dydt_func solver);
    void resolverContacts2(std::vector<RigidBody*>& bodies, float* y, float* ydot, float dt, dydt_func solver);

    void generateContacts(std::vector<RigidBody*>& bodies, float dt);
};