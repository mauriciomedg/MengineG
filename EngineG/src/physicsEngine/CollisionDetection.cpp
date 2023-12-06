#include "CollisionDetection.h"
#include "RigidBody.h"
#include "Contact.h"

namespace
{
    float transformToAxis(
        const CollisionBox& box,
        const glm::vec3& axis
    )
    {
        return
            box.halfSize.x * glm::abs(glm::dot(axis, box.getAxis(0))) +
            box.halfSize.y * glm::abs(glm::dot(axis, box.getAxis(1))) +
            box.halfSize.z * glm::abs(glm::dot(axis, box.getAxis(2)));
    }
}

void CollisionPrimitive::calculateInternals()
{
    transform = body->mWorldMat * offset;
}

void CollisionPrimitive::calculateInternals(const glm::mat4& transformMath)
{
    transform = transformMath * offset;
}

glm::vec3 CollisionPrimitive::getAxis(unsigned index) const
{
    return transform[index];
}

const glm::mat4& CollisionPrimitive::getTransform() const
{
    return transform;
}

void CollisionData::addContacts(unsigned count)
{
    // Reduce the number of contacts remaining, add number used
    contactsLeft -= count;
    contactCount += count;

    // Move the array forward
    contacts += count;
}

bool IntersectionTests::boxAndHalfSpace(
    const CollisionBox& box,
    const CollisionPlane& plane
)
{
    // Work out the projected radius of the box onto the plane direction
    //glm::vec3 vertexPos = box.transform * glm::vec4(box.halfSize, 0.0f);
    float projectedRadius = glm::length(box.halfSize);// vertexPos - plane.getAxis(3)); // transformToAxis(box, plane.direction);

    // Work out how far the box is from the origin
    auto P = plane.getAxis(3);
    auto boxCenter = box.getAxis(3);
    float boxDistance =
        glm::dot(plane.direction, boxCenter - P) - projectedRadius;

    // Check for the intersection
    return boxDistance <= plane.offset;
}

unsigned CollisionDetector::boxAndHalfSpace(
    const CollisionBox& box,
    const CollisionPlane& plane,
    CollisionData* data)
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0) return 0;

    // Check for intersection
    if (!IntersectionTests::boxAndHalfSpace(box, plane))
    {
        return 0;
    }

    // We have an intersection, so find the intersection points. We can make
    // do with only checking vertices. If the box is resting on a plane
    // or on an edge, it will be reported as four or two contact points.

    // Go through each combination of + and - for each half-size
    static float mults[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
                               {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

    Contact* contact = data->contacts;
    unsigned contactsUsed = 0;
    for (unsigned i = 0; i < 8; i++) {

        // Calculate the position of each vertex
        glm::vec4 vertexLocalPos(mults[i][0] * box.halfSize[0], mults[i][1] * box.halfSize[0], mults[i][2] * box.halfSize[0], 1.0f);
        glm::vec3 vertexPos = box.transform * vertexLocalPos;

        // Calculate the distance from the plane
        float vertexDistance = glm::dot(vertexPos - plane.getAxis(3), plane.direction);

        // Compare this to the plane's distance
        if (vertexDistance <= plane.offset)
        {
            // Create the contact data.

            // The contact point is halfway between the vertex and the
            // plane - we multiply the direction by half the separation
            // distance and add the vertex location.
            contact->contactPoint = vertexPos;// +plane.direction * ((plane.offset - vertexDistance) * 0.5f);
            contact->relContactPoint[0] = vertexPos - box.getAxis(3);
            contact->contactNormal = plane.direction;
            contact->contactDepth = plane.offset - vertexDistance;

            // Write the appropriate data
            contact->setBodyData(box.body, NULL,
                data->friction, data->restitution);

            // Move onto the next contact
            contact++;
            contactsUsed++;
            if (contactsUsed == (unsigned)data->contactsLeft) return contactsUsed;
        }
    }

    data->addContacts(contactsUsed);
    return contactsUsed;
}
