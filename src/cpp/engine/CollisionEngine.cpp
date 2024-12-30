#include "CollisionEngine.h"

void Octree::Build(const std::vector<Entity *> &entities)
{
}

CollisionEngine::CollisionEngine()
{
}

void CollisionEngine::Update(float delta)
{
    DetectCollisions();
    // TODO:
    // return collision object and call event listeners for the collisions
}

void CollisionEngine::DetectCollisions()
{
#ifdef USE_ENTT
    entt::entity i;
#else
    EntityID i = 0;
#endif
    for (auto e1 : m_CollisionEntities)
    {
#ifdef USE_ENTT
        i = e1;
#else
        i = e1->GetID();
#endif
        for (auto e2 : m_CollisionEntities)
        {
#ifdef USE_ENTT
            if (e2 <= i)
#else
            if (e2->GetID() <= i)
#endif
            {
                continue;
            }

            CheckCollision(e1, e2);
        }
    }
}

Collision CollisionEngine::CheckCollision(ENTITY a, ENTITY b) {
    Collision c = { V_ORIGIN };

    if (!HAS_COMPONENT(a, Components::Physics) || !HAS_COMPONENT(b, Components::Physics)) return c;
    Components::Collision& collisionA = GET_COMPONENT(a, Components::Collision);
    Components::Transform& transformA = GET_COMPONENT(a, Components::Transform);
    Components::Physics& physicsA = GET_COMPONENT(a, Components::Physics);

    Components::Collision& collisionB = GET_COMPONENT(b, Components::Collision);
    Components::Transform& transformB = GET_COMPONENT(b, Components::Transform);
    Components::Physics& physicsB = GET_COMPONENT(b, Components::Physics);

    glm::vec3 posA = transformA.GetPosition();
    glm::vec3 posB = transformB.GetPosition();
    glm::vec3 delta = posB - posA;
    float distanceSquared = glm::dot(delta, delta);
    float radiusSum = collisionA.Size.x + collisionB.Size.x;

    if (distanceSquared < radiusSum * radiusSum) {
        auto contactPoint = (posA + posB) / 2.0f;

        // Simple collision response: Reflect velocities
        physicsA.Velocity *= -1;
        physicsB.Velocity *= -1;
    }

    return c;
}

bool CollisionHelpers::IntersectSphereSphere(const Collision *a, const Collision *b)
{
    return false;
}
