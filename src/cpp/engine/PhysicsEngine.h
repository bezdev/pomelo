#pragma once

#include "engine/CollisionEngine.h"
#include "engine/ECS.h"
#include "engine/PhysicsJolt.h"
#include "util/Logger.h"
#include "util/Math.h"

class PhysicsEngine
{
  public:
    static PhysicsEngine *GetInstance()
    {
        if (!s_Instance)
            s_Instance = new PhysicsEngine();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    PhysicsEngine();

    void Initialize();

    void AddPhysicsEntity(ENTITY entity)
    {
        m_PhysicsEntities.push_back(entity);
        m_PhysicsJolt.AddEntity(entity);
    }

    void AddMotionEntity(ENTITY entity)
    {
        m_MotionEntities.push_back(entity);
    }

    void Update(float delta);

    void UpdateMotions(float delta);
    void UpdatePhysics(float delta);

  private:
    static PhysicsEngine *s_Instance;

    void UpdatePath(ENTITY entity, Components::Motion *motion, float delta);
    void UpdateOrbit(ENTITY entity, Components::Motion *motion, float delta);

    PhysicsJolt m_PhysicsJolt;
    std::vector<ENTITY> m_MotionEntities;
    std::vector<ENTITY> m_PhysicsEntities;

    std::vector<ENTITY> m_EntitiesCreatedQueue;
};
