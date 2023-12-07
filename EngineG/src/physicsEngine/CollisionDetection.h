#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>

class RigidBody;
class Contact;

class IntersectionTests;
class CollisionDetector;

class CollisionPrimitive
{
public:
    /**
     * This class exists to help the collision detector
     * and intersection routines, so they should have
     * access to its data.
     */
    friend class IntersectionTests;
    friend class CollisionDetector;

    RigidBody* body;

    /**
     * The offset of this primitive from the given rigid body.
     */
    glm::mat4 offset;

    /**
     * Calculates the internals for the primitive.
     */
    virtual void calculateInternals();
    virtual void calculateInternals(const glm::mat4& transformMath);
    /**
     * This is a convenience function to allow access to the
     * axis vectors in the transform for this primitive.
     */
    inline glm::vec3 getAxis(unsigned index) const;

    /**
     * Returns the resultant transform of the primitive, calculated from
     * the combined offset of the primitive and the transform
     * (orientation + position) of the rigid body to which it is
     * attached.
     */
    const glm::mat4& getTransform() const;

    bool mSimulatePhysics = false;

protected:
    /**
     * The resultant transform of the primitive. This is
     * calculated by combining the offset of the primitive
     * with the transform of the rigid body.
     */
    glm::mat4 transform;
};

/**
 * Represents a rigid body that can be treated as a sphere
 * for collision detection.
 */
class CollisionSphere : public CollisionPrimitive
{
public:
    virtual void calculateInternals() { CollisionPrimitive::calculateInternals(); }
    /**
     * The radius of the sphere.
     */
    float radius;
};

/**
 * The plane is not a primitive: it doesn't represent another
 * rigid body. It is used for contacts with the immovable
 * world geometry.
 */
class CollisionPlane : public CollisionPrimitive
{
public:
    virtual void calculateInternals() { CollisionPrimitive::calculateInternals(); }
    /**
     * The plane normal
     */
    glm::vec3 direction;

    /**
     * The distance of the plane from the origin.
     */
    float offset = 0.0f;
};

/**
 * Represents a rigid body that can be treated as an aligned bounding
 * box for collision detection.
 */
class CollisionBox : public CollisionPrimitive
{
public:
    virtual void calculateInternals() { CollisionPrimitive::calculateInternals(); }
    /**
     * Holds the half-sizes of the box along each of its local axes.
     */
    glm::vec3 halfSize;
};

/**
 * A wrapper class that holds fast intersection tests. These
 * can be used to drive the coarse collision detection system or
 * as an early out in the full collision tests below.
 */
class IntersectionTests
{
public:

    static bool sphereAndHalfSpace(
        const CollisionSphere& sphere,
        const CollisionPlane& plane);

    static bool sphereAndSphere(
        const CollisionSphere& one,
        const CollisionSphere& two);

    static bool boxAndBox(
        const CollisionBox& one,
        const CollisionBox& two);

    /**
     * Does an intersection test on an arbitrarily aligned box and a
     * half-space.
     *
     * The box is given as a transform matrix, including
     * position, and a vector of half-sizes for the extend of the
     * box along each local axis.
     *
     * The half-space is given as a direction (i.e. unit) vector and the
     * offset of the limiting plane from the origin, along the given
     * direction.
     */
    static bool boxAndHalfSpace(
        const CollisionBox& box,
        const CollisionPlane& plane);
};


/**
 * A helper structure that contains information for the detector to use
 * in building its contact data.
 */
struct CollisionData
{
    /**
     * Holds the base of the collision data: the first contact
     * in the array. This is used so that the contact pointer (below)
     * can be incremented each time a contact is detected, while
     * this pointer points to the first contact found.
     */
    Contact* contactArray;

    /** Holds the contact array to write into. */
    Contact* contacts;

    /** Holds the maximum number of contacts the array can take. */
    int contactsLeft;

    /** Holds the number of contacts found so far. */
    unsigned contactCount;

    /** Holds the friction value to write into any collisions. */
    float friction;

    /** Holds the restitution value to write into any collisions. */
    float restitution;

    /**
     * Holds the collision tolerance, even uncolliding objects this
     * close should have collisions generated.
     */
    float tolerance;

    /**
     * Checks if there are more contacts available in the contact
     * data.
     */
    bool hasMoreContacts()
    {
        return contactsLeft > 0;
    }

    /**
     * Resets the data so that it has no used contacts recorded.
     */
    void reset(unsigned maxContacts)
    {
        contactsLeft = maxContacts;
        contactCount = 0;
        contacts = contactArray;
    }

    /**
     * Notifies the data that the given number of contacts have
     * been added.
     */
    void addContacts(unsigned count);
};

/**
 * A wrapper class that holds the fine grained collision detection
 * routines.
 *
 * Each of the functions has the same format: it takes the details
 * of two objects, and a pointer to a contact array to fill. It
 * returns the number of contacts it wrote into the array.
 */
class CollisionDetector
{
public:

    static unsigned sphereAndHalfSpace(
        const CollisionSphere& sphere,
        const CollisionPlane& plane,
        CollisionData* data
    );

    static unsigned sphereAndTruePlane(
        const CollisionSphere& sphere,
        const CollisionPlane& plane,
        CollisionData* data
    );

    static unsigned sphereAndSphere(
        const CollisionSphere& one,
        const CollisionSphere& two,
        CollisionData* data
    );

    /**
     * Does a collision test on a collision box and a plane representing
     * a half-space (i.e. the normal of the plane
     * points out of the half-space).
     */
    static unsigned boxAndHalfSpace(
        const CollisionBox& box,
        const CollisionPlane& plane,
        CollisionData* data
    );

    static unsigned boxAndBox(
        const CollisionBox& one,
        const CollisionBox& two,
        CollisionData* data
    );

    static unsigned boxAndPoint(
        const CollisionBox& box,
        const glm::vec3& point,
        CollisionData* data
    );

    static unsigned boxAndSphere(
        const CollisionBox& box,
        const CollisionSphere& sphere,
        CollisionData* data
    );
};

struct CollisionDetection
{
    CollisionData cData;
    CollisionPrimitive* primitive;
    int nbPrimitive;
};