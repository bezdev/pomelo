#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <iostream>
#include <cstdarg>
#include <cstdint>
#include <thread>

#include "engine/CollisionEngine.h"
#include "engine/ECS.h"
#include "util/Logger.h"
#include "util/Math.h"

namespace Layers
{
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

/// Class that determines if two object layers can collide
class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
    virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
    {
        switch (inObject1)
        {
        case Layers::NON_MOVING:
            return inObject2 == Layers::MOVING; // Non moving only collides with moving
        case Layers::MOVING:
            return true; // Moving collides with everything
        default:
            JPH_ASSERT(false);
            return false;
        }
    }
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
    static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
    static constexpr JPH::BroadPhaseLayer MOVING(1);
    static constexpr unsigned int NUM_LAYERS(2);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BroadPhaseLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
    BroadPhaseLayerInterfaceImpl()
    {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
    }

    virtual unsigned int GetNumBroadPhaseLayers() const override
    {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
    {
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char *GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
    {
        switch ((BroadPhaseLayer::Type)inLayer)
        {
        case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
            return "NON_MOVING";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
            return "MOVING";
        default:
            JPH_ASSERT(false);
            return "INVALID";
        }
    }
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
    virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
    {
        switch (inLayer1)
        {
        case Layers::NON_MOVING:
            return inLayer2 == BroadPhaseLayers::MOVING;
        case Layers::MOVING:
            return true;
        default:
            JPH_ASSERT(false);
            return false;
        }
    }
};

class ContactListenerImpl : public JPH::ContactListener
{
public:
    virtual JPH::ValidateResult OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) override
    {
        std::cout << "Contact validate callback" << std::endl;

        // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
        return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    virtual void OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
    {
        std::cout << "A contact was added" << std::endl;
    }

    virtual void OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
    {
        std::cout << "A contact was persisted" << std::endl;
    }

    virtual void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override
    {
        std::cout << "A contact was removed" << std::endl;
    }
};

class BodyActivationListenerImpl : public JPH::BodyActivationListener
{
public:
    virtual void OnBodyActivated(const JPH::BodyID &inBodyID, uint64_t inBodyUserData) override
    {
        std::cout << "A body got activated" << std::endl;
    }

    virtual void OnBodyDeactivated(const JPH::BodyID &inBodyID, uint64_t inBodyUserData) override
    {
        std::cout << "A body went to sleep" << std::endl;
    }
};

class PhysicsEngine
{
public:
    static PhysicsEngine *GetInstance()
    {
        if (!s_Instance)
            s_Instance = new PhysicsEngine();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    static void JoltTraceImpl(const char *inFMT, ...) {
        // Format the message
        // va_list list;
        // va_start(list, inFMT);
        // char buffer[1024];
        // vsnprintf(buffer, sizeof(buffer), inFMT, list);
        // va_end(list);

        // Print to the TTY
        // std::cout << buffer << std::endl;
    };

    static bool JoltAssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, unsigned int inLine)
    {
        // Print to the TTY
        // std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr? inMessage : "") << std::endl;

        // Breakpoint
        return true;
    };

    PhysicsEngine();

    void Initialize();

    void AddMotionEntity(ENTITY entity) { m_MotionEntities.push_back(entity); }
    void AddPhysicsEntity(ENTITY entity) {
        std::cout << "Adding Entity: " << static_cast<uint32_t>(entity) << std::endl;
        m_PhysicsEntities.push_back(entity);
    }
    void AddCollisionEntity(ENTITY entity) { m_CollisionEngine.AddEntity(entity); }

    void Update(float delta);
    void UpdateMotions(float delta);
    void UpdatePhysics(float delta);
    void UpdateCollisions(float delta);
    void UpdateJoltPhysics(float delta);

private:
    static PhysicsEngine *s_Instance;

    void InitializeJolt();

    void UpdatePath(ENTITY entity, Components::Motion *motion, float delta);
    void UpdateOrbit(ENTITY entity, Components::Motion *motion, float delta);

    CollisionEngine m_CollisionEngine;
    std::vector<ENTITY> m_MotionEntities;
    std::vector<ENTITY> m_PhysicsEntities;

    // Jolt
    JPH::TempAllocatorImpl* m_TempAllocator;
    JPH::JobSystemThreadPool* m_JobSystem;
    BroadPhaseLayerInterfaceImpl m_BroadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl m_ObjectVsBroadPhaseLayerFilter;
    ObjectLayerPairFilterImpl m_ObjectLayerPairFilter;
    JPH::PhysicsSystem m_PhysicsSystem;
    ContactListenerImpl m_ContactListener;
    BodyActivationListenerImpl m_BodyActivationListener;
    JPH::BodyInterface* m_BodyInterface;

    // Hacks
    JPH::BodyID m_SphereId;
};
