#pragma once

#include "util/Math.h"

class Entity;

namespace Components
{
    struct Transform
    {
    // public:
        Transform() {}

        Transform(glm::vec3& position):
            Transform(position, VEC3(1.f))
        {}

        Transform(glm::vec3 position, glm::vec3 scale):
            Position(position),
            Scale(scale)
        {
            MM = glm::translate(glm::mat4(1.f), Position);
            MM = glm::scale(MM, Scale);
        }

        void SetPosition(const glm::vec3& position)
        {
            Position = position;
            MM[3] = glm::vec4(Position, 1.f);
        }

        void SetRotation(const glm::quat& rotation)
        {

        }

        void SetScale(const glm::vec3& scale)
        {

        }

    // private:
        glm::vec3 Position;
        glm::vec4 Rotation;
        glm::vec3 Scale;
        glm::mat4 MM;
        bool IsStale;
    };

    enum class MeshType
    {
        LINE,
        AXIS,
        PLANE,
        BOX,
        SPHERE,
        INSTANCED_BOX,
        BLENDER_AXIS,
        BLENDER_MONKEY,
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
