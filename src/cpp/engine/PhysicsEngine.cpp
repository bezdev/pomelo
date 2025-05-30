#include "PhysicsEngine.h"

#include "engine/EventDispatcher.h"
#include "util/Util.h"

using namespace JPH::literals;

PhysicsEngine *PhysicsEngine::s_Instance = nullptr;

PhysicsEngine::PhysicsEngine()
{
}

void PhysicsEngine::Initialize()
{
    m_PhysicsJolt.Initialize();

    EventDispatcher::GetInstance()->Subscribe(EventType::ENTITY_CREATED, [this](const Event &event) {
        if (std::holds_alternative<EntityEventData>(event.Data))
        {
            const auto &data = std::get<EntityEventData>(event.Data);
            m_EntitiesCreatedQueue.push_back(data.Entity);
        }
    });
}

void PhysicsEngine::Update(float delta)
{
    if (m_EntitiesCreatedQueue.size() > 0)
    {
        for (auto e : m_EntitiesCreatedQueue)
        {
            if (HAS_COMPONENT(e, Components::Motion))
            {
                AddMotionEntity(e);
            }
            if (HAS_COMPONENT(e, Components::CollisionBox) || HAS_COMPONENT(e, Components::CollisionSphere))
            {
                AddPhysicsEntity(e);
            }
        }
        m_EntitiesCreatedQueue.clear();

        m_PhysicsJolt.PostEntitiesAdded();
    }

    m_PhysicsJolt.Update(delta);
    UpdateMotions(delta);
}

void PhysicsEngine::UpdateMotions(float delta)
{
    for (auto e : m_MotionEntities)
    {
        Components::Motion &motion = GET_COMPONENT(e, Components::Motion);
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

void PhysicsEngine::UpdatePhysics(float delta)
{
    delta /= 1000.f;

    for (auto e : m_PhysicsEntities)
    {
        Components::Physics &physics = GET_COMPONENT(e, Components::Physics);
        Components::Transform &transform = GET_COMPONENT(e, Components::Transform);

        physics.Velocity += physics.Acceleration * delta;
        transform.SetPosition(transform.GetPosition() + physics.Velocity * delta);

        glm::vec3 rotationAxis = glm::cross(V_Z, physics.Direction);

        if (glm::length(rotationAxis) < 0.0005)
        {
            rotationAxis = glm::vec3(0, 1, 0);
        }

        float rotationAngle = acos(glm::dot(V_Z, physics.Direction));
        glm::quat rotationQuat = glm::angleAxis(rotationAngle, glm::normalize(rotationAxis));
        transform.SetRotation(rotationQuat);

        // TODO: make generic collision detection
        // if (transform.GetPosition().y < 0)
        // {
        //     physics.Velocity *= -1;
        //     transform.SetPosition(V_ORIGIN);
        // }
    }
}

void PhysicsEngine::UpdatePath(ENTITY entity, Components::Motion *motion, float delta)
{
    if (motion->Step >= 1.f)
        return;

    glm::vec3 direction = motion->Target - motion->Start;

    float speed = glm::length(direction) / motion->Time;
    float stepDistance = speed * delta;
    float step = stepDistance / glm::length(direction);

    motion->Step += step;
    glm::vec3 position(motion->Start + motion->Step * direction);

    if (motion->Step >= 1.f)
    {
        motion->Step = 1.f;
        position = motion->Target;
    }

    Components::Transform &transform = GET_COMPONENT(entity, Components::Transform);
    transform.SetPosition(position);
}

void PhysicsEngine::UpdateOrbit(ENTITY entity, Components::Motion *motion, float delta)
{
    Components::Transform &transform = GET_COMPONENT(entity, Components::Transform);

    // Calculate angle
    float angularSpeed = 2 * Constants::PI / motion->Time;
    float deltaAngle = delta * angularSpeed;

    // Calculate position relative to target
    glm::vec3 direction = (motion->Step == 0.f ? motion->Start : transform.GetPosition()) - motion->Target;
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
