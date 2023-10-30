#pragma once

#include "util/Util.h"
#include "util/Math.h"

class Entity;

namespace Components
{
    class Transform
    {
    public:
        Transform():
            Transform(V_ORIGIN)
        {}


        Transform(const VEC3& position):
            Transform(position, VEC3(1.f))
        {}

        Transform(const VEC3& position, const VEC3& scale):
            Transform(position, Q_DEFAULT, scale)
        {
        }

        Transform(const VEC3& position, const QUAT& rotation, const VEC3& scale):
            m_Position(position),
            m_Rotation(rotation),
            m_Scale(scale),
            m_MM(MAT4(1.f)),
            m_IsStale(false)
        {
            ComputeMM();
        }

        void SetPosition(const VEC3& position)
        {
            m_Position = position;
            m_MM[3] = VEC4(m_Position, 1.f);
        }

        void SetRotation(const QUAT& rotation)
        {
            m_Rotation = rotation;
            m_IsStale = true;
        }

        void SetScale(const VEC3& scale)
        {
            m_Scale = scale;
            m_IsStale = true;
        }

        VEC3& GetPosition() { return m_Position; }
        VEC3& GetScale() { return m_Scale; }

        MAT4& GetMM()
        {
            if (m_IsStale) ComputeMM();
            return m_MM;
        }

    private:
        void ComputeMM()
        {
            m_MM = glm::translate(MAT4(1.f), m_Position);
            m_MM *= glm::mat4_cast(m_Rotation);
            m_MM = glm::scale(m_MM, m_Scale);
            m_IsStale = false;
        }

        VEC3 m_Position;
        QUAT m_Rotation;
        VEC3 m_Scale;
        MAT4 m_MM;
        bool m_IsStale;
    };

    enum class MeshType
    {
        LINE,
        AXIS,
        PLANE,
        PLANE_MAP,
        PLANE_TEXTURE,
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
        Mesh(MeshType type, const char* filename):
            Type(type),
            Filename(filename)
        {}

        MeshType Type;
        const char* Filename;
        int Seed;
    };

    enum class MaterialType
    {
        SOLID_COLOR,
        PIXEL_COLOR,
        TEXTURE,
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

        Material(MaterialType type, glm::vec4 color, const char* filename):
            Type(type),
            Color(color),
            TextureFilename(filename)
        {}

        MaterialType Type;
        glm::vec4 Color;
        const char* TextureFilename;
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

        Motion(MotionType type, VEC3 start, VEC3 target, float time):
            Type(type),
            Start(start),
            Target(target),
            Step(0.f),
            Time(time)
        {}

        MotionType Type;
        VEC3 Start;
        VEC3 Target;
        float Step;
        float Time;
    };

    enum PhysicsType
    {
        NONE = 0,
        GRAVITY = 1 << 0
    };

    struct Physics
    {
        Physics():
            Velocity(VEC3(0.f, 0.f, 0.f)),
            Force(VEC3(0.f, 0.f, 0.f))
        {}

        Physics(PhysicsType type):
            Physics()
        {
            if (type & GRAVITY) Force += VEC3(0.f, -9.81f, 0.f);
        }
        VEC3 Velocity;
        VEC3 Force;
    };
}
