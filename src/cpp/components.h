#pragma once

#include <glm/glm.hpp>

class Entity;

namespace Components
{
    enum class MeshType
    {
        BOX,
        AXIS,
        COUNT
    };

    enum class MaterialType
    {
        SOLID_COLOR,
        PIXEL_COLOR,
        COUNT
    };

    struct Motion
    {
        Motion() {}
        Motion(glm::vec3 position):
            Position(position)
        {}

        glm::vec3 Position;
    };

    struct Mesh
    {
        Mesh() {}
        Mesh(MeshType type):
            Type(type)
        {}

        MeshType Type;
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
}
