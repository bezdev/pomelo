#pragma once

#include "engine/ECS.h"
#include "util/Math.h"
#include "util/Logger.h"

class BVH;

struct Collision
{
    // object a
    // object b
    // point of collision
    VEC3 CollisionPoint;
};

// enum class CollisionType
// {
//     STATIC,
//     DYNAMIC,
//     RAY
// };

struct AABB
{
    VEC3 Center;
    VEC3 Extends;
};

// struct Sphere
// {
//     glm::vec3 center;
//     float radius;
// };

// union
// {
//     AABB AABB;
//     Sphere Sphere;
// }
// BoundingVolume;

struct OctreeNode
{
    AABB BoundingBox;
    OctreeNode* Children[8];
    bool IsLeaf;

    OctreeNode()
    {
        for (int i = 0; i < 8; i++)
        {
            Children[i] = nullptr;
        }

        BoundingBox.Center = VEC3(0.f, 0.f, 0.f);
        BoundingBox.Extends = VEC3(0.f, 0.f, 0.f);
    }

    ~OctreeNode()
    {
        for (int i = 0; i < 8; i++)
        {
            delete Children[i];
            Children[i] = nullptr;
        }
    }
};

class Octree
{
public:
    Octree()
    {
    };

    void Build(const std::vector<Entity*>& entities);
    void AddEntity(Entity* entity)
    {
        
        // Components::Transform* t = 
    }
private:
    OctreeNode* m_Root;
};

class CollisionEngine
{
public:
// loops:
//   dynamic vs static
//   ra vs static
//   ra vs dynamic  
// static collision objects
//   - terrain, hieraghcical objects,
// dynamic collision objects
// ray casts
// data structures:
// - octree
// - BVH (bounding volume hierarchy)
// - grid sctuure

    static CollisionEngine* GetInstance()
    {
        if (!s_Instance) s_Instance = new CollisionEngine();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    CollisionEngine();

    void Update(float delta);
    void AddEntity(ENTITY entity) { m_CollisionEntities.push_back(entity); }
    void DetectCollisions();
    Collision CheckCollision(ENTITY a, ENTITY b);
private:
    static CollisionEngine* s_Instance;

    std::vector<ENTITY> m_CollisionEntities;
};

class CollisionHelpers
{
public:
    bool IntersectSphereSphere(const Collision* a, const Collision* b);
};
