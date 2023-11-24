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

    float contactDepth;
    int localContactId;

    bool isRestingContact = false;
    bool isStillInContact = false;
    void applyPositionChange(glm::vec3 linearChange[2],
        glm::vec3 angularChange[2],
        float penetration);

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
    void generateContacts(std::vector<RigidBody*>& bodies);
    void update(std::vector<RigidBody*>& bodies);
    
};