#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicsEngine.h"

#include "util.h"

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
        else if (motion.Type == Components::MotionType::ORBIT)
        {
            UpdateOrbit(e, &motion, delta);
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
    transform.SetPosition(position);
}

void PhysicsEngine::UpdateOrbit(Entity* entity, Components::Motion* motion, float delta)
{
    Components::Transform& transform = entity->GetComponent<Components::Transform>();

    // Calculate angle
    float angularSpeed = 2 * Constants::PI / motion->Time;
    float deltaAngle = delta  * angularSpeed;

    // Calculate position relative to target
    glm::vec3 direction = (motion->Step == 0.f ? motion->Start : transform.Position) - motion->Target;
    float distance = glm::length(direction);
    direction = glm::normalize(direction);

    // Rotate the relative position
    glm::vec3 rotationAxis = glm::normalize(glm::cross(motion->Start - motion->Target, glm::vec3(0.f, 0.f, 1.f)));
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deltaAngle, rotationAxis);
    glm::vec3 rotatedPosition = glm::vec3(rotation * glm::vec4(direction, 0.0f));

    // Update the position
    transform.SetPosition(motion->Target + rotatedPosition * distance);

    // Update the step
    motion->Step += delta / motion->Time;
    motion->Step = fmodf(motion->Step, 1.f);
}
