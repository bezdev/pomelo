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
    EntityID i = 0;
    for (auto e1 : m_CollisionEntities)
    {
        i = e1->GetID();
        for (auto e2 : m_CollisionEntities)
        {
            if (e2->GetID() <= i)
            {
                continue;
            }

            CheckCollision(e1, e2);
        }
    }
}

Collision CollisionEngine::CheckCollision(Entity *a, Entity *b) {
    Collision c = { V_ORIGIN };

    if (!a->HasComponent<Components::Physics>() || !b->HasComponent<Components::Physics>()) return c;
    Components::Collision& collisionA = a->GetComponent<Components::Collision>();
    Components::Transform& transformA = a->GetComponent<Components::Transform>();
    Components::Physics& physicsA = a->GetComponent<Components::Physics>();

    Components::Collision& collisionB = b->GetComponent<Components::Collision>();
    Components::Transform& transformB = b->GetComponent<Components::Transform>();
    Components::Physics& physicsB = b->GetComponent<Components::Physics>();

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
