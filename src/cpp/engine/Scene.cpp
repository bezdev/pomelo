#include "engine/Scene.h"
#include "Scene.h"
#include "engine/EntityFactory.h"
#include "engine/EventDispatcher.h"
#include "engine/PhysicsEngine.h"
#include "render/Renderer.h"

Scene::Scene()
{
}

void Scene::Load()
{
    Renderer::GetInstance()->ClearEntities();
    Renderer::GetInstance()->LoadGUI();
}

void SceneManager::CreateSandboxScene()
{
    Scene s;

    glm::vec3 a(1.f, 0.f, 0.f);
    glm::vec3 b(0.f, 1.f, 0.f);
    // EntityFactory::CreateLine(V_ORIGIN, a, V_YELLOW);

    // EntityFactory::CreateLine(V_ORIGIN, b, V_YELLOW);
    // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));
    // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));
    // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));

    EntityFactory::CreateLine(V_ORIGIN, a + b, V_COLOR_BLUE);
    EntityFactory::CreateAxis(V_ORIGIN);

    // s.CreateEntity()
    //     .AddComponent<Components::Transform>(V_ORIGIN, glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f,
    //     0.0f)), VEC3(1.f)) .AddComponent<Components::Mesh>(Components::MeshType::PLANE_MAP)
    //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 1.f, 1.f, 1.0f));

    auto e1 = CREATE_ENTITY();
    ADD_COMPONENT(e1, Components::Transform, V_ORIGIN, glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), VEC3(1.f));
    ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::PLANE_MAP);
    ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 1.f, 1.f, 1.0f));

    // s.CreateEntity()
    //     .AddComponent<Components::Transform>(V_ORIGIN)
    //     .AddComponent<Components::Mesh>(Components::MeshType::BOX)
    //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 0.f, 0.f, 1.0f))
    //     .AddComponent<Components::Motion>(Components::MotionType::ORBIT, glm::vec3(-10.f, 0.f, 0.f), glm::vec3(0.f,
    //     0.f, 0.f), 5000);

    auto e2 = CREATE_ENTITY();
    ADD_COMPONENT(e2, Components::Transform, V_ORIGIN);
    ADD_COMPONENT(e2, Components::Mesh, Components::MeshType::SPHERE);
    ADD_COMPONENT(e2, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 0.f, 1.f, 1.0f));
    ADD_COMPONENT(e2, Components::Motion, Components::MotionType::ORBIT, glm::vec3(10.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 5000);

    auto e3 = CREATE_ENTITY();
    ADD_COMPONENT(e3, Components::Transform, V_ORIGIN + VEC3(0.f, 10.f, 0.f));
    ADD_COMPONENT(e3, Components::Mesh, Components::MeshType::SPHERE);
    ADD_COMPONENT(e3, Components::Material, Components::MaterialType::SOLID_COLOR, V_COLOR_BLUE);
    ADD_COMPONENT(e3, Components::Physics, Components::GRAVITY);

    auto e4 = CREATE_ENTITY();
    ADD_COMPONENT(e4, Components::Transform, V_ORIGIN + VEC3(-5.f, 5.f, 0.f));
    ADD_COMPONENT(e4, Components::Mesh, Components::MeshType::PLANE);
    ADD_COMPONENT(e4, Components::Material, Components::MaterialType::TEXTURE, V_COLOR_WHITE, "assets/fonts/default.png");

    auto e5 = CREATE_ENTITY();
    ADD_COMPONENT(e5, Components::Transform, VEC3(10.f, 10.f, 0), VEC3(100.f, 100.f, 100.f));
    ADD_COMPONENT(e5,
        Components::Text,
        TextManager::GetInstance()->CreateText(std::string("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"))->GetTextID());

    // s.CreateEntity()
    //     .AddComponent<Components::Transform>(p)
    //     .AddComponent<Components::Mesh>(Components::MeshType::INSTANCED_BOX)
    //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 1.f, 0.f, 1.0f));

    // s.CreateEntity()
    //     .AddComponent<Components::Transform>(p)
    //     .AddComponent<Components::Mesh>(Components::MeshType::BLENDER_AXIS)
    //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 1.f, 0.f, 1.0f));

    Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 10.f, 15.f), V_ORIGIN);
    Camera::GetInstance()->SetCameraType(CameraType::FREE_LOOK);
    s.Load();
}

void SceneManager::CreateGameScene()
{
    Scene s;
    EntityFactory::CreateAxis(V_ORIGIN);

    auto car = CREATE_ENTITY();
    ADD_COMPONENT(car, Components::Transform, V_ORIGIN, VEC3(1.f, 1.f, 5.f));
    ADD_COMPONENT(car, Components::Mesh, Components::MeshType::BOX);
    ADD_COMPONENT(car, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 0.f, 0.f, 1.0f));
    // ADD_COMPONENT(car, Components::Physics);
    //  .AddComponent<Components::Motion>(Components::MotionType::ORBIT, glm::vec3(10.f, 0.f, 0.f), glm::vec3(0.f, 0.f,
    //  0.f), 5000);

    auto e1 = CREATE_ENTITY();
    ADD_COMPONENT(e1, Components::Transform, V_ORIGIN, glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), VEC3(1.f));
    ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::PLANE_MAP);
    ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 1.f, 1.f, 1.0f));

    // s.CreateEntity()
    //     .AddComponent<Components::Transform>(V_ORIGIN + VEC3(10.f, 0.f, 0.f))
    //     .AddComponent<Components::Mesh>(Components::MeshType::BLENDER_AXIS)
    //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 1.f, 0.f, 1.0f));

    // InputManager::GetInstance()->RegisterCallback(InputEvent::KEY_W, [&](InputEvent event, InputData data) {
    //     if (data.Action == InputAction::UP)
    //     {
    //         // m_IsDrawWireFrame = !m_IsDrawWireFrame;
    //     }
    // });

    Components::Physics *p = &GET_COMPONENT(car, Components::Physics);
    EventDispatcher::GetInstance()->Subscribe(EventType::ON_UPDATE, [p](const Event &e) {
        if (std::holds_alternative<InputEventData>(e.Data))
        {
            float c_acceleration = 6;
            float current_acceleration = 0;
            float turnRate = Constants::PI / 1000;

            const auto &data = std::get<OnUpdateEventData>(e.Data);
            const float delta = data.DeltaTime;

            if (InputManager::GetInstance()->IsKeyDown(InputEvent::KEY_J))
            {
                float turnAngle = 1 * turnRate * delta;
                glm::quat rotation = glm::angleAxis(turnAngle, glm::vec3(0.0f, 1.0f, 0.0f));
                p->Direction = glm::normalize(rotation * p->Direction);
                p->Velocity = glm::length(p->Velocity) * p->Direction;
            }
            if (InputManager::GetInstance()->IsKeyDown(InputEvent::KEY_L))
            {
                float turnAngle = -1 * turnRate * delta;
                glm::quat rotation = glm::angleAxis(turnAngle, glm::vec3(0.0f, 1.0f, 0.0f));
                p->Direction = glm::normalize(rotation * p->Direction);
                p->Velocity = glm::length(p->Velocity) * p->Direction;
            }

            float acceleration = c_acceleration;
            if (InputManager::GetInstance()->IsKeyDown(InputEvent::KEY_I))
            {
                acceleration = c_acceleration;
            }
            else
            {
                acceleration = -c_acceleration * 3;
            }

            if (glm::dot(glm::normalize(p->Velocity), p->Direction) < 0)
            {
                acceleration = 0.f;
                p->Velocity = V_ORIGIN;
            }

            p->Acceleration = acceleration * p->Direction;
        }
    });

    Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 10.f, 15.f), V_ORIGIN);
    // Camera::GetInstance()->SetCameraType(CameraType::FOLLOW_TARGET);
    Camera::GetInstance()->SetCameraType(CameraType::FREE_LOOK);

    s.Load();
}

void SceneManager::CreateCollisionScene(bool hasPhysics, bool hasCollisions)
{
    // Scene s;

    // EntityFactory::CreateAxis(V_ORIGIN);

    // int NUM_PAIRS = 100;
    // for (int i = 0; i < NUM_PAIRS; i++)
    // {
    //     float space = 2.f;
    //     float y = (-NUM_PAIRS / 2 + .5) * space + (i * space);

    //     if (hasPhysics)
    //     {
    //         if (hasCollisions)
    //         {
    //             auto e1 = CREATE_ENTITY();
    //             ADD_COMPONENT(e1, Components::Transform, VEC3(10.f, y, 0.f));
    //             ADD_COMPONENT(e1, Components::Physics, VEC3(-5.f, 0.f, 0.f));
    //             ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::SPHERE);
    //             ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR, V_COLOR_BLUE);
    //             ADD_COMPONENT(e1, Components::Collision, Components::CollisionType::SPHERE, V_ORIGIN, VEC2(.5f, .5f));

    //             auto e2 = CREATE_ENTITY();
    //             ADD_COMPONENT(e2, Components::Transform, VEC3(-10.f, y, 0.f));
    //             ADD_COMPONENT(e2, Components::Physics, VEC3(5.f, 0.f, 0.f));
    //             ADD_COMPONENT(e2, Components::Mesh, Components::MeshType::SPHERE);
    //             ADD_COMPONENT(e2, Components::Material, Components::MaterialType::SOLID_COLOR, V_COLOR_RED);
    //             ADD_COMPONENT(e2, Components::Collision, Components::CollisionType::SPHERE, V_ORIGIN, VEC2(.5f, .5f));
    //         }
    //         else
    //         {
    //             auto e1 = CREATE_ENTITY();
    //             ADD_COMPONENT(e1, Components::Transform, VEC3(10.f, y, 0.f));
    //             ADD_COMPONENT(e1, Components::Physics, VEC3(-5.f, 0.f, 0.f));
    //             ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::SPHERE);
    //             ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR, V_COLOR_BLUE);

    //             auto e2 = CREATE_ENTITY();
    //             ADD_COMPONENT(e2, Components::Transform, VEC3(-10.f, y, 0.f));
    //             ADD_COMPONENT(e2, Components::Physics, VEC3(5.f, 0.f, 0.f));
    //             ADD_COMPONENT(e2, Components::Mesh, Components::MeshType::SPHERE);
    //             ADD_COMPONENT(e2, Components::Material, Components::MaterialType::SOLID_COLOR, V_COLOR_RED);
    //         }
    //     }
    //     else
    //     {
    //         auto e1 = CREATE_ENTITY();
    //         ADD_COMPONENT(e1, Components::Transform, VEC3(10.f, y, 0.f));
    //         ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::SPHERE);
    //         ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR, V_COLOR_BLUE);

    //         auto e2 = CREATE_ENTITY();
    //         ADD_COMPONENT(e2, Components::Transform, VEC3(-10.f, y, 0.f));
    //         ADD_COMPONENT(e2, Components::Mesh, Components::MeshType::SPHERE);
    //         ADD_COMPONENT(e2, Components::Material, Components::MaterialType::SOLID_COLOR, V_COLOR_RED);
    //     }
    // }

    // // s.CreateEntity()
    // //     .AddComponent<Components::Transform>(glm::vec3(10.f, 0.f, 0.f))
    // //     .AddComponent<Components::Physics>(VEC3(-5.f, 0.f, 0.f))
    // //     .AddComponent<Components::Mesh>(Components::MeshType::SPHERE)
    // //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, V_COLOR_BLUE)
    // //     .AddComponent<Components::Collision>(Components::CollisionType::SPHERE, V_ORIGIN, VEC2(.5f, .5f));

    // // s.CreateEntity()
    // //     .AddComponent<Components::Transform>(glm::vec3(-10.f, 0.f, 0.f))
    // //     .AddComponent<Components::Physics>(VEC3(5.f, 0.f, 0.f))
    // //     .AddComponent<Components::Mesh>(Components::MeshType::SPHERE)
    // //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, V_COLOR_RED)
    // //     .AddComponent<Components::Collision>(Components::CollisionType::SPHERE, V_ORIGIN, VEC2(.5f, .5f));

    // Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 50.f, 50.f), glm::vec3(0.f, 0.f, 0.f));
    // s.Load();
}

void SceneManager::CreateJoltHelloWorldScene()
{
    Scene s;

    EntityFactory::CreateAxis(V_ORIGIN);

    float degrees = 45.0f;
    glm::quat tilt1 = glm::angleAxis(glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f));
    auto floor1 = CREATE_ENTITY();
    ADD_COMPONENT(floor1, Components::Transform, VEC3(0, 0, 0), tilt1, VEC3(100.f, 1.f, 100.f));
    ADD_COMPONENT(floor1, Components::Mesh, Components::MeshType::BOX);
    ADD_COMPONENT(floor1, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
    ADD_COMPONENT(floor1,
        Components::CollisionBox,
        VEC3(50.f, 1.f, 50.f),
        VEC3(0.f),
        glm::quat(1, 0, 0, 0),
        Components::CollisionMotionType::Static,
        Components::CollisionActivationType::DontActivate,
        Components::CollisionLayer::NON_MOVING);
    glm::quat tilt2 = glm::angleAxis(glm::radians(-degrees), glm::vec3(1.0f, 0.0f, 0.0f));
    auto floor2 = CREATE_ENTITY();
    ADD_COMPONENT(floor2, Components::Transform, VEC3(0, 0, 0), tilt2, VEC3(100.f, 1.f, 100.f));
    ADD_COMPONENT(floor2, Components::Mesh, Components::MeshType::BOX);
    ADD_COMPONENT(floor2, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
    ADD_COMPONENT(floor2,
        Components::CollisionBox,
        VEC3(50.f, 1.f, 50.f),
        VEC3(0.f),
        glm::quat(1, 0, 0, 0),
        Components::CollisionMotionType::Static,
        Components::CollisionActivationType::DontActivate,
        Components::CollisionLayer::NON_MOVING);

    size_t SPHERE_COUNT = 50;
    for (size_t i = 0; i < SPHERE_COUNT; i++)
    {
        // if (i < (SPHERE_COUNT / 2))
        // {
        //     glm::vec3 p(0, 60, -100 + i * (200.f / SPHERE_COUNT));
        //     auto sphere = CREATE_ENTITY();
        //     ADD_COMPONENT(sphere, Components::Transform, p, VEC3(.5f, .5f, .5f));
        //     ADD_COMPONENT(sphere, Components::Motion, Components::MotionType::PATH, p, VEC3(p.x, 0.f, p.z), 5000);
        //     ADD_COMPONENT(sphere, Components::Mesh, Components::MeshType::SPHERE);
        //     ADD_COMPONENT(sphere, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(1.f, 0.f, 0.f, 1.f));
        // }
        // else
        // {
        // glm::vec3 p(RANDOM_FLOAT(-50, 50), 20, RANDOM_FLOAT(-50, 50));
        glm::vec3 p(0, 60, -100 + i * (200.f / SPHERE_COUNT));
        auto sphere = CREATE_ENTITY();
        ADD_COMPONENT(sphere, Components::Transform, p, VEC3(.5f, .5f, .5f));
        ADD_COMPONENT(sphere, Components::Physics, VEC3(0.f, -2.f, 0.f));
        ADD_COMPONENT(sphere, Components::Mesh, Components::MeshType::SPHERE);
        // ADD_COMPONENT(
        //     sphere, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
        ADD_COMPONENT(sphere, Components::Material, Components::MaterialType::SOLID_COLOR, glm::vec4(1.f, 0.f, 0.f, 1.f));
        Components::CollisionSphere &collisionSphere = ADD_COMPONENT(sphere, Components::CollisionSphere);
        collisionSphere.Radius = .5f;
        collisionSphere.MotionType = Components::CollisionMotionType::Dynamic;
        collisionSphere.ActivationType = Components::CollisionActivationType::Activate;
        collisionSphere.Layer = Components::CollisionLayer::MOVING;
        // }
    }
    // auto sphere = CREATE_ENTITY();
    // ADD_COMPONENT(sphere, Components::Transform, VEC3(0, 20, 0), VEC3(.5f, .5f, .5f));
    // ADD_COMPONENT(sphere, Components::Physics, VEC3(0.f, -5.f, 0.f));
    // ADD_COMPONENT(sphere, Components::Mesh, Components::MeshType::SPHERE);
    // ADD_COMPONENT(sphere, Components::Material, Components::MaterialType::SOLID_COLOR,
    //               glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
    // Components::CollisionSphere &collisionSphere = ADD_COMPONENT(sphere, Components::CollisionSphere);
    // collisionSphere.Radius = .5f;
    // collisionSphere.MotionType = Components::CollisionMotionType::Dynamic;
    // collisionSphere.ActivationType = Components::CollisionActivationType::Activate;
    // collisionSphere.Layer = Components::CollisionLayer::MOVING;

    // PhysicsEngine::GetInstance()->AddPhysicsEntity(floor);
    // PhysicsEngine::GetInstance()->AddPhysicsEntity(sphere);

    // Camera::GetInstance()->SetLookAt(glm::vec3(100.f, 100.f, 100.f), glm::vec3(0.f, 0.f, 0.f));
    Camera::GetInstance()->SetLookAt(glm::vec3(20.f, 50.f, 0.f), glm::vec3(0.f, 50.f, 0.f));
    Camera::GetInstance()->SetCameraType(CameraType::FREE_LOOK);
    s.Load();
}
