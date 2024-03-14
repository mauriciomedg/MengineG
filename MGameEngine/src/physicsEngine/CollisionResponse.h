#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class RigidBody;
//class Contact;
class CollisionData;
class CollisionPrimitive;

//struct ContactGenRegistration
//{
//    Contact* gen;
//    ContactGenRegistration* next;
//};

class CollisionResponse
{
    //enum {
    //    MaxContacts = 100
    //};

    //ContactGenRegistration* mFirstContact;

    //Contact* mContacts;
    //Contact* mContactItt;
public:

    typedef void (*dydt_func)(float* y, float* ydot, float deltaT, int size);

    CollisionResponse();
    //void generateContacts(std::vector<RigidBody*>& bodies);
    
    void solvePositions(CollisionData* cData, std::vector<CollisionPrimitive*>& primitives);
    
    void solvePositions(CollisionData* cData, float dt);

    void update(CollisionData* cData, std::vector<CollisionPrimitive*>& primitives);
    
};