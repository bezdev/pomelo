#pragma once

#include "engine/ECS.h"
#include "util/Math.h"
#include "util/Logger.h"

class Octree;
class BVH;
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
private:
    static CollisionEngine* s_Instance;
};
