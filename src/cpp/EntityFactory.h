#pragma once

#include "glm/glm.hpp"
#include "ECS.h"

namespace EntityFactory
{
    static Entity& CreateEntity() { return ECS::GetInstance()->CreateEntity(); };

    static Entity& CreateLine(glm::vec3& position, glm::vec3& target, glm::vec4& color)
    {
        auto& e = CreateEntity()
            .AddComponent<Components::Transform>(position)
            .AddComponent<Components::Mesh>(Components::MeshType::LINE)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, color);
        e.GetComponent<Components::Transform>().SetScale(target);

        return e;
    }

    static void CreateAxis(glm::vec3& position, glm::vec3& axis)
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

    static void CreateAxis(glm::vec3& position)
    {
        glm::vec3 axis = glm::vec3(0.f, 0.f, 0.f);
        CreateAxis(position, axis);
    }
};