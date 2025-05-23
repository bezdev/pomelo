#pragma once

// clang-format off
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

#include "engine/ECS.h"
#include "util/Logger.h"
#include "util/Math.h"

#define VEC3_TO_JPH_VEC3(v) JPH::Vec3(v.x, v.y, v.z)
#define JPH_VEC3_TO_VEC3(v) VEC3(v.GetX(), v.GetY(), v.GetZ())
#define VEC3_TO_JPH_RVEC3(v) JPH::RVec3(static_cast<JPH::Real>((v).x), \
                                        static_cast<JPH::Real>((v).y), \
                                        static_cast<JPH::Real>((v).z))
// clang-format on

namespace Layers
{
static constexpr JPH::ObjectLayer NON_MOVING = 0;
static constexpr JPH::ObjectLayer MOVING = 1;
static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
}; // namespace Layers

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
}; // namespace BroadPhaseLayers

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
    virtual JPH::ValidateResult OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset,
                                                  const JPH::CollideShapeResult &inCollisionResult) override
    {
        // LOGD("OnContactValidate");

        // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
        return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    virtual void OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
    {
        // LOGD("OnContactAdded");
    }

    virtual void OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
    {
        // LOGD("OnContactPersisted");
    }

    virtual void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override
    {
        // LOGD("OnContactRemoved");
    }
};

class BodyActivationListenerImpl : public JPH::BodyActivationListener
{
  public:
    virtual void OnBodyActivated(const JPH::BodyID &inBodyID, uint64_t inBodyUserData) override
    {
        // LOGD("OnBodyActivated");
    }

    virtual void OnBodyDeactivated(const JPH::BodyID &inBodyID, uint64_t inBodyUserData) override
    {
        // LOGD("OnBodyDeactivated");
    }
};

class PhysicsJolt
{
  public:
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
        LOGD("JoltAssertFailedImpl inFile: %s, inLine: %d, inExpression: %s, inMessage; %s", inFile, inLine, inExpression, inMessage);

        // Breakpoint
        return true;
    };

    PhysicsJolt();

    void Initialize();
    void AddEntity(ENTITY entity);
    void Update(float delta);
    void PostEntitiesAdded();

  private:
    JPH::TempAllocatorImpl *m_TempAllocator;
    JPH::JobSystemThreadPool *m_JobSystem;
    BroadPhaseLayerInterfaceImpl m_BroadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl m_ObjectVsBroadPhaseLayerFilter;
    ObjectLayerPairFilterImpl m_ObjectLayerPairFilter;
    JPH::PhysicsSystem m_PhysicsSystem;
    ContactListenerImpl m_ContactListener;
    BodyActivationListenerImpl m_BodyActivationListener;
    JPH::BodyInterface *m_BodyInterface;

    std::unordered_map<JPH::BodyID, ENTITY> m_BodyToEntityMap;
};
