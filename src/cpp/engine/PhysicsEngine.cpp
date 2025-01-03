#include "PhysicsEngine.h"

#include "util/Util.h"

using namespace JPH::literals;

PhysicsEngine *PhysicsEngine::s_Instance = nullptr;

PhysicsEngine::PhysicsEngine()
{
}

void PhysicsEngine::Initialize()
{
    InitializeJolt();
}

void PhysicsEngine::InitializeJolt()
{
    // Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
    // This needs to be done before any other Jolt function is called.
    JPH::RegisterDefaultAllocator();

    // Install trace and assert callbacks
    JPH::Trace = JoltTraceImpl;
    JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = JoltAssertFailedImpl;)

    // Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
    // It is not directly used in this example but still required.
    JPH::Factory::sInstance = new JPH::Factory();

    // Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
    // If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
    // If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
    JPH::RegisterTypes();

    // We need a temp allocator for temporary allocations during the physics update. We're
    // pre-allocating 10 MB to avoid having to do allocations during the physics update.
    // B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
    // If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
    // malloc / free.
    // JPH::TempAllocatorImpl temp_allocator(10 * 1024 * 1024)
    m_TempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);

    // Maximum amount of jobs to allow
    int cMaxPhysicsJobs = 2048;
    // Maximum amount of barriers to allow
    int cMaxPhysicsBarriers = 8;

    // We need a job system that will execute physics jobs on multiple threads. Typically
    // you would implement the JobSystem interface yourself and let Jolt Physics run on top
    // of your own job scheduler. JobSystemThreadPool is an example implementation.
    // JPH::JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
    m_JobSystem = new JPH::JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

    // This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    const unsigned int cMaxBodies = 1024;

    // This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
    const unsigned int cNumBodyMutexes = 0;

    // This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
    // body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
    // too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    const unsigned int cMaxBodyPairs = 1024;

    // This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
    // number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
    const unsigned int cMaxContactConstraints = 1024;

    // Create mapping table from object layer to broadphase layer
    // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    // Also have a look at BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a simpler interface.
    // BroadPhaseLayerInterfaceImpl m_BroadPhaseLayerInterface;

    // // Create class that filters object vs broadphase layers
    // // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    // // Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
    // ObjectVsBroadPhaseLayerFilterImpl m_ObjectVsBroadPhaseLayerFilter;

    // // Create class that filters object vs object layers
    // // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
    // // Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.
    // ObjectLayerPairFilterImpl m_ObjectLayerPairFilter;

    // Now we can create the actual physics system.
    m_PhysicsSystem.Init(
        cMaxBodies,
        cNumBodyMutexes,
        cMaxBodyPairs,
        cMaxContactConstraints,
        m_BroadPhaseLayerInterface,
        m_ObjectVsBroadPhaseLayerFilter,
        m_ObjectLayerPairFilter);

    // A body activation listener gets notified when bodies activate and go to sleep
    // Note that this is called from a job so whatever you do here needs to be thread safe.
    // Registering one is entirely optional.
    m_PhysicsSystem.SetBodyActivationListener(&m_BodyActivationListener);

    // A contact listener gets notified when bodies (are about to) collide, and when they separate again.
    // Note that this is called from a job so whatever you do here needs to be thread safe.
    // Registering one is entirely optional.
    m_PhysicsSystem.SetContactListener(&m_ContactListener);

    // The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
    // variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
    m_BodyInterface = &m_PhysicsSystem.GetBodyInterface();

    // Next we can create a rigid body to serve as the floor, we make a large box
    // Create the settings for the collision volume (the shape).
    // Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
    JPH::BoxShapeSettings floorShapeSettings(JPH::Vec3(100.0f, 1.0f, 100.0f));
    floorShapeSettings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

    // Create the shape
    JPH::ShapeSettings::ShapeResult floorShapeResult = floorShapeSettings.Create();
    JPH::ShapeRefC floorShape = floorShapeResult.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

    // Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
    JPH::BodyCreationSettings floorSettings(
        floorShape,
        JPH::RVec3(0.0_r, -1.0_r, 0.0_r),
        JPH::Quat::sIdentity(),
        JPH::EMotionType::Static,
        Layers::NON_MOVING);

    // Create the actual rigid body
    JPH::Body *floor = m_BodyInterface->CreateBody(floorSettings); // Note that if we run out of bodies this can return nullptr

    // Add it to the world
    m_BodyInterface->AddBody(floor->GetID(), JPH::EActivation::DontActivate);

    // Now create a dynamic body to bounce on the floor
    // Note that this uses the shorthand version of creating and adding a body to the world
    JPH::BodyCreationSettings sphereSettings(
        new JPH::SphereShape(0.5f),
        JPH::RVec3(0.0_r, 20.0_r, 0.0_r),
        JPH::Quat::sIdentity(),
        JPH::EMotionType::Dynamic,
        Layers::MOVING);
    m_SphereId = m_BodyInterface->CreateAndAddBody(
        sphereSettings,
        JPH::EActivation::Activate);

    // Now you can interact with the dynamic body, in this case we're going to give it a velocity.
    // (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
    m_BodyInterface->SetLinearVelocity(m_SphereId, JPH::Vec3(0.0f, -5.0f, 0.0f));

    // We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
    const float cDeltaTime = 1.0f / 60.0f;

    // Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
    // You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
    // Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
    m_PhysicsSystem.OptimizeBroadPhase();

    // unsigned int step = 0;
    // while (m_BodyInterface->IsActive(sphereId))
    // {
    //     // Next step
    //     ++step;

    //     // Output current position and velocity of the sphere
    //     JPH::RVec3 position = m_BodyInterface->GetCenterOfMassPosition(sphereId);
    //     JPH::Vec3 velocity = m_BodyInterface->GetLinearVelocity(sphereId);
    //     std::cout << "Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;

    //     // If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
    //     const int cCollisionSteps = 1;

    //     // Step the world
    //     m_PhysicsSystem.Update(cDeltaTime, cCollisionSteps, &temp_allocator, &job_system);
    // }

    // Cleanup
    // // Remove the sphere from the physics system. Note that the sphere itself keeps all of its state and can be re-added at any time.
    // m_BodyInterface->RemoveBody(sphereId);

    // // Destroy the sphere. After this the sphere ID is no longer valid.
    // m_BodyInterface->DestroyBody(sphereId);

    // // Remove and destroy the floor
    // m_BodyInterface->RemoveBody(floor->GetID());
    // m_BodyInterface->DestroyBody(floor->GetID());

    // // Unregisters all types with the factory and cleans up the default material
    // JPH::UnregisterTypes();

    // // Destroy the factory
    // delete JPH::Factory::sInstance;
    // JPH::Factory::sInstance = nullptr;
}

void PhysicsEngine::Update(float delta)
{
    UpdateMotions(delta);
    UpdatePhysics(delta);
    UpdateCollisions(delta);

    UpdateJoltPhysics(delta);
}

void PhysicsEngine::UpdateMotions(float delta)
{
    for (auto e : m_MotionEntities)
    {
        Components::Motion &motion = GET_COMPONENT(e, Components::Motion);
        if (motion.Type == Components::MotionType::PATH)
        {
            UpdatePath(e, &motion, delta);
        }
        else if (motion.Type == Components::MotionType::ORBIT)
        {
            UpdateOrbit(e, &motion, delta);
        }
    }
}

void PhysicsEngine::UpdatePhysics(float delta)
{
    return;
    delta /= 1000.f;

    for (auto e : m_PhysicsEntities)
    {
        Components::Physics &physics = GET_COMPONENT(e, Components::Physics);
        Components::Transform &transform = GET_COMPONENT(e, Components::Transform);

        physics.Velocity += physics.Acceleration * delta;
        transform.SetPosition(transform.GetPosition() + physics.Velocity * delta);

        glm::vec3 rotationAxis = glm::cross(V_Z, physics.Direction);

        if (glm::length(rotationAxis) < 0.0005)
        {
            rotationAxis = glm::vec3(0, 1, 0);
        }

        float rotationAngle = acos(glm::dot(V_Z, physics.Direction));
        glm::quat rotationQuat = glm::angleAxis(rotationAngle, glm::normalize(rotationAxis));
        transform.SetRotation(rotationQuat);

        // TODO: make generic collision detection
        // if (transform.GetPosition().y < 0)
        // {
        //     physics.Velocity *= -1;
        //     transform.SetPosition(V_ORIGIN);
        // }
    }
}

void PhysicsEngine::UpdateCollisions(float delta)
{
    m_CollisionEngine.Update(delta);
}

void PhysicsEngine::UpdateJoltPhysics(float delta)
{
    delta /= 1000.f;

    static float accumulatedTime = 0.0f; // Tracks the accumulated time
    const float fixedTimestep = 1.0f / 60.0f; // Fixed timestep (60Hz)

    // Accumulate the delta time
    accumulatedTime += delta;

    // Only run the physics update when accumulated time exceeds the fixed timestep
    while (accumulatedTime >= fixedTimestep && m_BodyInterface->IsActive(m_SphereId))
    {
        // Reduce accumulated time by the fixed timestep
        accumulatedTime -= fixedTimestep;

        // std::cout << "Delta: " << delta << ", Accumulated Time: " << accumulatedTime << std::endl;

        // Output current position and velocity of the sphere
        JPH::RVec3 position = m_BodyInterface->GetCenterOfMassPosition(m_SphereId);
        JPH::Vec3 velocity = m_BodyInterface->GetLinearVelocity(m_SphereId);
        std::cout << "Step: Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), "
                  << "Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;

        // If you take larger steps than 1/60th of a second, you may need multiple collision steps to keep the simulation stable.
        const int cCollisionSteps = 1;

        GET_COMPONENT(m_PhysicsEntities[0], Components::Transform).SetPosition(glm::vec3(position.GetX(), position.GetY(), position.GetZ()));

        // Step the world
        m_PhysicsSystem.Update(fixedTimestep, cCollisionSteps, m_TempAllocator, m_JobSystem);
    }
}

void PhysicsEngine::UpdatePath(ENTITY entity, Components::Motion *motion, float delta)
{
    if (motion->Step >= 1.f)
        return;

    glm::vec3 direction = motion->Target - motion->Start;

    float speed = glm::length(direction) / motion->Time;
    float stepDistance = speed * delta;
    float step = stepDistance / glm::length(direction);

    motion->Step += step;
    glm::vec3 position(motion->Start + motion->Step * direction);

    if (motion->Step >= 1.f)
    {
        motion->Step = 1.f;
        position = motion->Target;
    }

    Components::Transform &transform = GET_COMPONENT(entity, Components::Transform);
    transform.SetPosition(position);
}

void PhysicsEngine::UpdateOrbit(ENTITY entity, Components::Motion *motion, float delta)
{
    Components::Transform &transform = GET_COMPONENT(entity, Components::Transform);

    // Calculate angle
    float angularSpeed = 2 * Constants::PI / motion->Time;
    float deltaAngle = delta * angularSpeed;

    // Calculate position relative to target
    glm::vec3 direction = (motion->Step == 0.f ? motion->Start : transform.GetPosition()) - motion->Target;
    float distance = glm::length(direction);
    direction = glm::normalize(direction);

    // Rotate the relative position
    glm::vec3 rotationAxis = glm::normalize(glm::cross(motion->Start - motion->Target, glm::vec3(0.f, 0.f, 1.f)));
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deltaAngle, rotationAxis);
    glm::vec3 rotatedPosition = glm::vec3(rotation * glm::vec4(direction, 0.0f));

    // Update the position
    transform.SetPosition(motion->Target + rotatedPosition * distance);

    // Update the step
    motion->Step += delta / motion->Time;
    motion->Step = fmodf(motion->Step, 1.f);
}
