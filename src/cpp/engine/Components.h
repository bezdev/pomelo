#pragma once

#include "util/Util.h"
#include "util/Math.h"

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
        Mesh(MeshType type, const char* name):
            Type(type),
            Name(name)
        {}

        MeshType Type;
        const char* Name;
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

        Material(MaterialType type, glm::vec4 color, const char* name):
            Type(type),
            Color(color),
            Name(name)
        {}

        MaterialType Type;
        glm::vec4 Color;
        const char* Name;
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
        Physics(VEC3 velocity, VEC3 acceleration, VEC3 direction):
            Velocity(velocity),
            Acceleration(acceleration),
            Direction(direction)
        {}

        Physics():
            Physics(
                VEC3(0.f, 0.f, 0.f),
                VEC3(0.f, 0.f, 0.f),
                VEC3(0.f, 0.f, 1.f)
            )
        {}

        Physics(PhysicsType type):
            Physics()
        {
            if (type & GRAVITY) Acceleration += VEC3(0.f, -9.81f, 0.f);
        }

        Physics(VEC3 velocity):
            Physics(velocity, V_ORIGIN, VEC3(0.f, 0.f, 1.f))
        {}

        VEC3 Velocity;
        VEC3 Acceleration;
        VEC3 Direction;
    };

    enum CollisionType
    {
        SPHERE,
        BOX
    };

    struct Collision
    {
        // TODO: call other constructor
        Collision()
        {}

        Collision(CollisionType type, VEC3 position, VEC2 size):
            Type(type),
            Position(position),
            Size(size)
        {}

        CollisionType Type;
        VEC3 Position;
        VEC2 Size;
    };

    enum FontType
    {
        DEFAULT = 0
    };

    struct Text
    {
        Text() {}
        Text(int id):
            ID(id)
        {}

        int ID;
    };

    enum class CollisionMotionType : uint8_t
    {
        Static = 0,
        Kinematic,
        Dynamic,
    };

    enum class CollisionActivationType
    {
        Activate = 0,
        DontActivate
    };

    enum class CollisionLayer
    {
        NON_MOVING = 0,
        MOVING = 1,
        NUM_LAYERS = 2,
    };

    struct CollisionSphere
    {
        float Radius;
        CollisionMotionType MotionType;
        CollisionActivationType ActivationType;
        CollisionLayer Layer;
    };

    struct CollisionBox
    {
        VEC3 Extents;
        CollisionMotionType MotionType;
        CollisionActivationType ActivationType;
        CollisionLayer Layer;
    };
}
