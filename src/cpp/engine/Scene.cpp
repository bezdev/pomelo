#include "scene.h"

#include "PhysicsEngine.h"
#include "EntityFactory.h"

Scene::Scene()
{
}

void Scene::Load()
{
    Renderer::GetInstance()->LoadEntities(ECS::GetInstance()->GetEntities());

    for (auto e : ECS::GetInstance()->GetEntitiesWithComponents<Components::Motion>())
    {
        PhysicsEngine::GetInstance()->AddMotionEntity(e);
    }

    for (auto e : ECS::GetInstance()->GetEntitiesWithComponents<Components::Physics>())
    {
        PhysicsEngine::GetInstance()->AddPhysicsEntity(e);
    }
}

void SceneManager::CreateSandboxScene()
{
        Scene s;

        glm::vec3 a (1.f, 0.f, 0.f);
        glm::vec3 b (0.f, 1.f, 0.f);
        // EntityFactory::CreateLine(V_ORIGIN, a, V_YELLOW);

        // EntityFactory::CreateLine(V_ORIGIN, b, V_YELLOW);
        EntityFactory::CreateLine(V_ORIGIN, a + b, V_COLOR_BLUE);
        // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));
        // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));
        // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));

        EntityFactory::CreateAxis(V_ORIGIN);

        // s.CreateEntity()
        //     .AddComponent<Components::Transform>(V_ORIGIN, glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), VEC3(1.f))
        //     .AddComponent<Components::Mesh>(Components::MeshType::PLANE_MAP)
        //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 1.f, 1.f, 1.0f));

        // s.CreateEntity()
        //     .AddComponent<Components::Transform>(V_ORIGIN)
        //     .AddComponent<Components::Mesh>(Components::MeshType::BOX)
        //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 0.f, 0.f, 1.0f))
        //     .AddComponent<Components::Motion>(Components::MotionType::ORBIT, glm::vec3(-10.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 5000);

        s.CreateEntity()
            .AddComponent<Components::Transform>(V_ORIGIN)
            .AddComponent<Components::Mesh>(Components::MeshType::SPHERE)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 0.f, 1.f, 1.0f))
            .AddComponent<Components::Motion>(Components::MotionType::ORBIT, glm::vec3(10.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 5000);

        s.CreateEntity()
            .AddComponent<Components::Transform>(V_ORIGIN + VEC3(0.f, 10.f, 0.f))
            .AddComponent<Components::Mesh>(Components::MeshType::SPHERE)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, V_COLOR_BLUE)
            .AddComponent<Components::Physics>(Components::GRAVITY);

        s.CreateEntity()
            .AddComponent<Components::Transform>(V_ORIGIN + VEC3(-5.f, 5.f, 0.f))
            .AddComponent<Components::Mesh>(Components::MeshType::PLANE)
            .AddComponent<Components::Material>(Components::MaterialType::TEXTURE, V_COLOR_WHITE, "assets/fonts/default.png");

        s.CreateEntity()
            .AddComponent<Components::Transform>(VEC3(10.f, 10.f, 0), VEC3(100.f, 100.f, 100.f))
            // .AddComponent<Components::Text>(TextManager::GetInstance()->AddText(std::string("bezdev")));
            // .AddComponent<Components::Text>(TextManager::GetInstance()->AddText(std::string("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz")));
            .AddComponent<Components::Text>(TextManager::GetInstance()->AddText(std::string("    .      ")));

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
