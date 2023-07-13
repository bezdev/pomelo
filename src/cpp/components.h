#pragma once

#include <glm/glm.hpp>

class Entity;

namespace Components
{
    struct Transform
    {
        Transform() {}
        Transform(glm::vec3 position):
            Position(position)
        {}

        glm::vec3 Position;
        // glm::vec3 Rotation;
        glm::vec3 Scale;

    };

    enum class MeshType
    {
        BOX,
        AXIS,
        COUNT
    };

    struct Mesh
    {
        Mesh() {}
        Mesh(MeshType type):
            Type(type)
        {}

        MeshType Type;
    };

    enum class MaterialType
    {
        SOLID_COLOR,
        PIXEL_COLOR,
        COUNT
    };

    struct Material
    {
        Material() {}
        Material(MaterialType type):
            Type(type)
        {}

        Material(MaterialType type, glm::vec4 color):
            Type(type),
            Color(color)
        {}

        MaterialType Type;
        glm::vec4 Color;
    };

    struct Physics
    {
        glm::vec3 Velocity;
        glm::vec3 Acceleration;
    };

    enum class MotionType
    {
        PATH,
        ORBIT,
        COUNT
    };

    struct Motion
    {
        Motion() {};

        Motion(MotionType type, glm::vec3 start, glm::vec3 target, float time):
            Type(type),
            Start(start),
            Target(target),
            Step(0.f),
            Time(time)
        {}

        MotionType Type;
        glm::vec3 Start;
        glm::vec3 Target;
        float Step;
        float Time;
    };
}
