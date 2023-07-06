#pragma once

#include <glm/glm.hpp>

class Entity;

namespace Components
{
    enum class MeshType
    {
        BOX,
        AXIS,
    };

    enum class MaterialType
    {
        SolidColor
    };

    struct Motion
    {
        Motion(glm::vec3 position):
            Position(position)
        {}

        glm::vec3 Position;
    };

    struct Mesh
    {
        Mesh(MeshType type):
            Type(type)
        {}

        MeshType Type;
    };

    struct Material
    {
        Material(MaterialType type, glm::vec4 color):
            Type(type),
            Color(color)
        {}

        MaterialType Type;
        glm::vec4 Color;
    };
}
