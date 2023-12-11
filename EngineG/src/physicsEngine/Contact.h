#pragma once
#include <glm/gtc/type_ptr.hpp>

struct RigidBody;

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
    glm::vec3 relContactPoint[2];

    float friction;
    float restitution;

    float contactDepth;
    int localContactId;

    bool isRestingContact = false;
    bool isStillInContact = false;

    void setBodyData(RigidBody* one, RigidBody* two,
        float friction, float restitution);

    void applyPositionChange(glm::vec3 linearChange[2],
        glm::vec3 angularChange[2],
        float penetration);

    void applyVelocityChange(glm::vec3 velocityChange[2],
        glm::vec3 rotationChange[2]);

    Contact* nextContact;
};