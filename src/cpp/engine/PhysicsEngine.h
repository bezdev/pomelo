#pragma once

#include "engine/ECS.h"
#include "util/Math.h"
#include "util/Logger.h"

class PhysicsEngine
{
public:
    static PhysicsEngine* GetInstance()
    {
        if (!s_Instance) s_Instance = new PhysicsEngine();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    PhysicsEngine();

    void AddMotionEntity(Entity* entity) { m_MotionEntities.push_back(entity); }
    void AddPhysicsEntity(Entity* entity) { m_PhysicsEntities.push_back(entity); }
    void AddCollisionEntity(Entity* entity) { m_CollisionEntities.push_back(entity); }
    void Update(float delta);
    void UpdateMotions(float delta);
    void UpdatePhysics(float delta);
    void UpdateCollisions(float delta);
private:
    static PhysicsEngine* s_Instance;

    bool CheckCollision(Entity* a, Entity* b);
    void UpdatePath(Entity* entity, Components::Motion* motion, float delta);
    void UpdateOrbit(Entity* entity, Components::Motion* motion, float delta);

    std::vector<Entity*> m_MotionEntities;
    std::vector<Entity*> m_PhysicsEntities;
    std::vector<Entity*> m_CollisionEntities;
};
