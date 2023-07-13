#include "PhysicsEngine.h"

PhysicsEngine* PhysicsEngine::s_Instance = nullptr;

PhysicsEngine::PhysicsEngine()
{
}

void PhysicsEngine::UpdateMotions(float delta)
{
    for (auto e : m_MotionEntities)
    {
        Components::Motion& motion = e->GetComponent<Components::Motion>();
        if (motion.Type == Components::MotionType::PATH)
        {
            UpdatePath(e, &motion, delta);
        }
    }
}

void PhysicsEngine::UpdatePath(Entity* entity, Components::Motion* motion, float delta)
{
    if (motion->Step >= 1.f) return;

    glm::vec3 direction = motion->Target - motion->Start;

    float speed = glm::length(direction) / motion->Time;
    float stepDistance = speed * delta;
    float step = stepDistance / glm::length(direction);

    motion->Step += step;
    glm::vec3 position(motion->Start + motion->Step * direction);

    if (motion->Step >= 1.f) {
        motion->Step = 1.f;
        position = motion->Target;
    }

    Components::Transform& transform = entity->GetComponent<Components::Transform>();
    transform.Position = position;
}
