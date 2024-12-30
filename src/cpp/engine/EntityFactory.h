#pragma once

#include "util/Math.h"
#include "engine/ECS.h"

namespace EntityFactory
{
#ifdef USE_ENTT
    static entt::entity CreateEntity() {
        return CREATE_ENTITY();
    }

    static entt::entity CreateLine(const VEC3& position, const VEC3& target, const VEC4& color)
    {
        auto e = CreateEntity();
        ENTT::GetInstance()->emplace<Components::Transform>(e, position);
        ENTT::GetInstance()->emplace<Components::Mesh>(e, Components::MeshType::LINE);
        ENTT::GetInstance()->emplace<Components::Material>(e, Components::MaterialType::SOLID_COLOR, color);

        return e;
    }

    static void CreateAxis(const VEC3& position, const VEC3& axis)
    {
        auto e1 = CreateEntity();
        ENTT::GetInstance()->emplace<Components::Transform>(e1, position);
        ENTT::GetInstance()->emplace<Components::Mesh>(e1, Components::MeshType::AXIS);
        ENTT::GetInstance()->emplace<Components::Material>(e1, Components::MaterialType::PIXEL_COLOR);

        auto e2 = CreateEntity();
        ENTT::GetInstance()->emplace<Components::Transform>(e2, position + glm::vec3(1.f, 0.f, 0.f), glm::vec3(.1f));
        ENTT::GetInstance()->emplace<Components::Mesh>(e2, Components::MeshType::BOX);
        ENTT::GetInstance()->emplace<Components::Material>(e2, Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 0.f, 0.f, 1.0f));

        auto e3 = CreateEntity();
        ENTT::GetInstance()->emplace<Components::Transform>(e3, position + glm::vec3(0.f, 1.f, 0.f), glm::vec3(.1f));
        ENTT::GetInstance()->emplace<Components::Mesh>(e3, Components::MeshType::BOX);
        ENTT::GetInstance()->emplace<Components::Material>(e3, Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 0.f, 1.f, 1.0f));

        auto e4 = CreateEntity();
        ENTT::GetInstance()->emplace<Components::Transform>(e4, position + glm::vec3(0.f, 0.f, 1.f), glm::vec3(.1f));
        ENTT::GetInstance()->emplace<Components::Mesh>(e4, Components::MeshType::BOX);
        ENTT::GetInstance()->emplace<Components::Material>(e4, Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 1.f, 0.f, 1.0f));
    }
#else
    static Entity& CreateEntity() { return ECS::GetInstance()->CreateEntity(); };

    static Entity& CreateLine(const VEC3& position, const VEC3& target, const VEC4& color)
    {
        auto& e = CreateEntity()
            .AddComponent<Components::Transform>(position)
            .AddComponent<Components::Mesh>(Components::MeshType::LINE)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, color);
        e.GetComponent<Components::Transform>().SetScale(target);

        return e;
    }

    static void CreateAxis(const VEC3& position, const VEC3& axis)
    {
        CreateEntity()
            .AddComponent<Components::Transform>(position)
            .AddComponent<Components::Mesh>(Components::MeshType::AXIS)
            .AddComponent<Components::Material>(Components::MaterialType::PIXEL_COLOR);

        CreateEntity()
            .AddComponent<Components::Transform>(position + glm::vec3(1.f, 0.f, 0.f), glm::vec3(.1f))
            .AddComponent<Components::Mesh>(Components::MeshType::BOX)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 0.f, 0.f, 1.0f));

        CreateEntity()
            .AddComponent<Components::Transform>(position + glm::vec3(0.f, 1.f, 0.f), glm::vec3(.1f))
            .AddComponent<Components::Mesh>(Components::MeshType::BOX)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 0.f, 1.f, 1.0f));

        CreateEntity()
            .AddComponent<Components::Transform>(position + glm::vec3(0.f, 0.f, 1.f), glm::vec3(.1f))
            .AddComponent<Components::Mesh>(Components::MeshType::BOX)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 1.f, 0.f, 1.0f));
    }
#endif

    static void CreateAxis(const VEC3& position)
    {
        glm::vec3 axis = glm::vec3(0.f, 0.f, 0.f);
        CreateAxis(position, axis);
    }
};