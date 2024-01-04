#include "CollisionDetection.h"
#include "RigidBody.h"
#include "Contact.h"

///////////////////////////////////////////////////
namespace MMG
{
    struct Object {
        glm::vec2 center;    // Center point for object
        float radius;        // Radius of object bounding sphere
        Object* pNextObject; // Pointer to next object when linked into list
    };

    struct Node {
        glm::vec2 center;  // Center point of octree node (not strictly needed)
        float halfWidth;   // Half the width of the node volume (not strictly needed)
        Node* pChild[4];   // Pointers to the eight children nodes
        Object* pObjList;  // Linked list of objects contained at this node
    };

    // Preallocates an octree down to a specific depth
    Node* BuildOctree(const glm::vec2& center, float halfWidth, int stopDepth)
    {
        if (stopDepth < 0)
            return nullptr;
                
        // Construct and fill in ‘root’ of this subtree
        Node* pNode = new Node;
        pNode->center = center;
        pNode->halfWidth = halfWidth;
        pNode->pObjList = nullptr;
        
        // Recursively construct the eight children of the subtree
        glm::vec2 offset;
        float step = halfWidth * 0.5f;
        for (int i = 0; i < 4; ++i)
        {
            offset.x = ((i & 1) ? step : -step);
            offset.y = ((i & 2) ? step : -step);
            //offset.z = ((i & 4) ? step : -step);
            pNode->pChild[i] = BuildOctree(center + offset, step, stopDepth - 1);
        }

        return pNode;
    }

    void InsertObject(Node* pTree, Object* pObject)
    {
        int index = 0, straddle = 0;
        // Compute the octant number [0..7] the object sphere center is in
        // If straddling any of the dividing x, y, or z planes, exit directly
        for (int i = 0; i < 2; ++i)
        {
            float delta = pObject->center[i] - pTree->center[i];
            if (glm::abs(delta) <= pObject->radius)
            {
                straddle = 1;
                break;
            }
            if (delta > 0.0f) index |= (1 << i); // YX
        }

        if (!straddle && pTree->pChild[index])
        {
            //if (pTree->pChild[index] == nullptr)
            //{
            //    //Node* pNode = new Node;
            //    //pTree->pChild[index] = pNode;
            //    //pNode->center = center;
            //    //pNode->halfWidth = halfWidth;
            //    //pNode->pObjList = nullptr;
            //}
            // Fully contained in existing child node; insert in that subtree
            InsertObject(pTree->pChild[index], pObject);
        }
        else {
            // Straddling, or no child node to descend into, so
            // link object into linked list at this node
            pObject->pNextObject = pTree->pObjList;
            pTree->pObjList = pObject;
        }
    }
}

///////////////////////////////////////////////////
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

    static inline bool overlapOnAxis(
        const CollisionBox& one,
        const CollisionBox& two,
        const glm::vec3& axis,
        const glm::vec3& toCentre
    )
    {
        // Project the half-size of one onto axis
        float oneProject = transformToAxis(one, axis);
        float twoProject = transformToAxis(two, axis);

        // Project this onto the axis
        float distance = glm::abs(glm::dot(toCentre, axis));

        // Check for overlap
        return (distance < oneProject + twoProject);
    }

    static inline float penetrationOnAxis(
        const CollisionBox& one,
        const CollisionBox& two,
        const glm::vec3& axis,
        const glm::vec3& toCentre
    )
    {
        // Project the half-size of one onto axis
        float oneProject = transformToAxis(one, axis);
        float twoProject = transformToAxis(two, axis);

        // Project this onto the axis
        float distance = glm::abs(glm::dot(toCentre, axis));

        // Return the overlap (i.e. positive indicates
        // overlap, negative indicates separation).
        return oneProject + twoProject - distance;
    }

    static inline bool tryAxis(
        const CollisionBox& one,
        const CollisionBox& two,
        glm::vec3 axis,
        const glm::vec3& toCentre,
        unsigned index,

        // These values may be updated
        float& smallestPenetration,
        unsigned& smallestCase
    )
    {
        // Make sure we have a normalized axis, and don't check almost parallel axes
        if (glm::dot(axis, axis) < 0.0001) return true;
        axis = glm::normalize(axis);

        float penetration = penetrationOnAxis(one, two, axis, toCentre);

        if (penetration < 0) return false;
        if (penetration < smallestPenetration) {
            smallestPenetration = penetration;
            smallestCase = index;
        }
        return true;
    }

    void fillPointFaceBoxBox(
        const CollisionBox& one,
        const CollisionBox& two,
        const glm::vec3& toCentre,
        CollisionData* data,
        unsigned best,
        float pen
    )
    {
        // This method is called when we know that a vertex from
        // box two is in contact with box one.

        Contact* contact = data->contacts;

        // We know which axis the collision is on (i.e. best),
        // but we need to work out which of the two faces on
        // this axis.
        glm::vec3 normal = one.getAxis(best);
        if (glm::dot(one.getAxis(best), toCentre) > 0.0f)
        {
            normal = normal * -1.0f;
        }

        // Work out which vertex of box two we're colliding with.
        // Using toCentre doesn't work!
        glm::vec3 vertex = two.halfSize;
        if (glm::dot(two.getAxis(0), normal) < 0) vertex.x = -vertex.x;
        if (glm::dot(two.getAxis(1), normal) < 0) vertex.y = -vertex.y;
        if (glm::dot(two.getAxis(2), normal) < 0) vertex.z = -vertex.z;

        // Create the contact data
        contact->contactNormal = normal;
        contact->contactDepth = pen;
        contact->contactPoint = two.getTransform() * glm::vec4(vertex, 1.0f);
        contact->setBodyData(one.body, two.body,
            data->friction, data->restitution);
    }

    static inline glm::vec3 contactPoint(
        const glm::vec3& pOne,
        const glm::vec3& dOne,
        float oneSize,
        const glm::vec3& pTwo,
        const glm::vec3& dTwo,
        float twoSize,

        // If this is true, and the contact point is outside
        // the edge (in the case of an edge-face contact) then
        // we use one's midpoint, otherwise we use two's.
        bool useOne)
    {
        glm::vec3 toSt, cOne, cTwo;
        float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
        float denom, mua, mub;

        smOne = glm::dot(dOne, dOne);
        smTwo = glm::dot(dTwo, dTwo);
        dpOneTwo = glm::dot(dTwo, dOne);

        toSt = pOne - pTwo;
        dpStaOne = glm::dot(dOne, toSt);
        dpStaTwo = glm::dot(dTwo, toSt);

        denom = smOne * smTwo - dpOneTwo * dpOneTwo;

        // Zero denominator indicates parrallel lines
        if (glm::abs(denom) < 0.0001f) {
            return useOne ? pOne : pTwo;
        }

        mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
        mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

        // If either of the edges has the nearest point out
        // of bounds, then the edges aren't crossed, we have
        // an edge-face contact. Our point is on the edge, which
        // we know from the useOne parameter.
        if (mua > oneSize ||
            mua < -oneSize ||
            mub > twoSize ||
            mub < -twoSize)
        {
            return useOne ? pOne : pTwo;
        }
        else
        {
            cOne = pOne + dOne * mua;
            cTwo = pTwo + dTwo * mub;

            return cOne * 0.5f + cTwo * 0.5f;
        }
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

#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) return 0;

unsigned CollisionDetector::boxAndBox(
    const CollisionBox& one,
    const CollisionBox& two,
    CollisionData* data)
{
    // Find the vector between the two centres
    glm::vec3 toCentre = two.getAxis(3) - one.getAxis(3);

    // We start assuming there is no contact
    float pen = 10000000.0f;
    unsigned best = 0xffffff;

    // Now we check each axes, returning if it gives us
    // a separating axis, and keeping track of the axis with
    // the smallest contactDepth otherwise.
    CHECK_OVERLAP(one.getAxis(0), 0);
    CHECK_OVERLAP(one.getAxis(1), 1);
    CHECK_OVERLAP(one.getAxis(2), 2);

    CHECK_OVERLAP(two.getAxis(0), 3);
    CHECK_OVERLAP(two.getAxis(1), 4);
    CHECK_OVERLAP(two.getAxis(2), 5);

    // Store the best axis-major, in case we run into almost
    // parallel edge collisions later
    unsigned bestSingleAxis = best;

    CHECK_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(0)), 6);
    CHECK_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(1)), 7);
    CHECK_OVERLAP(glm::cross(one.getAxis(0), two.getAxis(2)), 8);
    CHECK_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(0)), 9);
    CHECK_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(1)), 10);
    CHECK_OVERLAP(glm::cross(one.getAxis(1), two.getAxis(2)), 11);
    CHECK_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(0)), 12);
    CHECK_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(1)), 13);
    CHECK_OVERLAP(glm::cross(one.getAxis(2), two.getAxis(2)), 14);

    // Make sure we've got a result.
    assert(best != 0xffffff);

    // We now know there's a collision, and we know which
    // of the axes gave the smallest contactDepth. We now
    // can deal with it in different ways depending on
    // the case.
    if (best < 3)
    {
        // We've got a vertex of box two on a face of box one.
        fillPointFaceBoxBox(one, two, toCentre, data, best, pen);
        data->addContacts(1);
        return 1;
    }
    else if (best < 6)
    {
        // We've got a vertex of box one on a face of box two.
        // We use the same algorithm as above, but swap around
        // one and two (and therefore also the vector between their
        // centres).
        fillPointFaceBoxBox(two, one, toCentre * -1.0f, data, best - 3, pen);
        data->addContacts(1);
        return 1;
    }
    else
    {
        // We've got an edge-edge contact. Find out which axes
        best -= 6;
        unsigned oneAxisIndex = best / 3;
        unsigned twoAxisIndex = best % 3;
        glm::vec3 oneAxis = one.getAxis(oneAxisIndex);
        glm::vec3 twoAxis = two.getAxis(twoAxisIndex);
        glm::vec3 axis = glm::cross(oneAxis, twoAxis);
        axis = glm::normalize(axis);

        // The axis should point from box one to box two.
        if (glm::dot(axis, toCentre) > 0) axis = axis * -1.0f;

        // We have the axes, but not the edges: each axis has 4 edges parallel
        // to it, we need to find which of the 4 for each object. We do
        // that by finding the point in the centre of the edge. We know
        // its component in the direction of the box's collision axis is zero
        // (its a mid-point) and we determine which of the extremes in each
        // of the other axes is closest.
        glm::vec3 ptOnOneEdge = one.halfSize;
        glm::vec3 ptOnTwoEdge = two.halfSize;
        for (unsigned i = 0; i < 3; i++)
        {
            if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
            else if (glm::dot(one.getAxis(i), axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

            if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
            else if (glm::dot(two.getAxis(i), axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
        }

        // Move them into world coordinates (they are already oriented
        // correctly, since they have been derived from the axes).
        ptOnOneEdge = one.getTransform() * glm::vec4(ptOnOneEdge, 1.0f);
        ptOnTwoEdge = two.getTransform() * glm::vec4(ptOnTwoEdge, 1.0f);

        // So we have a point and a direction for the colliding edges.
        // We need to find out point of closest approach of the two
        // line-segments.
        glm::vec3 vertex = contactPoint(
            ptOnOneEdge, oneAxis, one.halfSize[oneAxisIndex],
            ptOnTwoEdge, twoAxis, two.halfSize[twoAxisIndex],
            bestSingleAxis > 2
        );

        // We can fill the contact.
        Contact* contact = data->contacts;

        contact->contactDepth = pen;
        contact->contactNormal = axis;
        contact->contactPoint = vertex;
        contact->setBodyData(one.body, two.body,
            data->friction, data->restitution);
        data->addContacts(1);
        return 1;
    }
    return 0;
}
#undef CHECK_OVERLAP

unsigned CollisionDetector::boxAndPoint(
    const CollisionBox& box,
    const glm::vec3& point,
    CollisionData* data
)
{
    // Transform the point into box coordinates
    glm::vec3 relPt = glm::inverse(box.getTransform()) * glm::vec4(point, 1.0f);

    glm::vec3 normal;

    // Check each axis, looking for the axis on which the
    // contactDepth is least deep.
    float min_depth = box.halfSize.x - glm::abs(relPt.x);
    if (min_depth < 0.0f) return 0;
    normal = box.getAxis(0) * ((relPt.x < 0.f) ? -1.0f : 1.0f);

    float depth = box.halfSize.y - glm::abs(relPt.y);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(1) * ((relPt.y < 0.f) ? -1.0f : 1.0f);
    }

    depth = box.halfSize.z - glm::abs(relPt.z);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(2) * ((relPt.z < 0.f) ? -1.0f : 1.0f);
    }

    // Compile the contact
    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = point;
    contact->contactDepth = min_depth;

    // Note that we don't know what rigid body the point
    // belongs to, so we just use NULL. Where this is called
    // this value can be left, or filled in.
    contact->setBodyData(box.body, NULL,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}
