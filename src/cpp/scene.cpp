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
}

void SceneManager::CreateSandboxScene()
{
        Scene s;

        glm::vec3 a (1.f, 0.f, 0.f);
        glm::vec3 b (0.f, 1.f, 0.f);
        EntityFactory::CreateLine(V_ORIGIN, a, glm::vec4(1.f, 1.f, 0.f, 1.f));
        EntityFactory::CreateLine(V_ORIGIN, b, glm::vec4(1.f, 1.f, 0.f, 1.f));
        EntityFactory::CreateLine(V_ORIGIN, a + b, glm::vec4(1.f, 0.f, 0.f, 1.f));
        // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));
        // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));
        // EntityFactory::CreateLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec4(1.0f, 1.f, 0.f, 1.0f));

        EntityFactory::CreateAxis(V_ORIGIN);

        s.CreateEntity()
            .AddComponent<Components::Transform>(V_ORIGIN, glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), VEC3(10.f))
            .AddComponent<Components::Mesh>(Components::MeshType::PLANE)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 0.f, 0.f, 1.0f));

        s.CreateEntity()
            .AddComponent<Components::Transform>(V_ORIGIN)
            .AddComponent<Components::Mesh>(Components::MeshType::BOX)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 0.f, 0.f, 1.0f))
            .AddComponent<Components::Motion>(Components::MotionType::ORBIT, glm::vec3(-10.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 5000);

        s.CreateEntity()
            .AddComponent<Components::Transform>(V_ORIGIN)
            .AddComponent<Components::Mesh>(Components::MeshType::SPHERE)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 0.f, 1.f, 1.0f))
            .AddComponent<Components::Motion>(Components::MotionType::ORBIT, glm::vec3(10.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 5000);

        // s.CreateEntity()
        //     .AddComponent<Components::Transform>(p)
        //     .AddComponent<Components::Mesh>(Components::MeshType::INSTANCED_BOX)
        //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 1.f, 0.f, 1.0f));

        // s.CreateEntity()
        //     .AddComponent<Components::Transform>(p)
        //     .AddComponent<Components::Mesh>(Components::MeshType::BLENDER_AXIS)
        //     .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 1.f, 0.f, 1.0f));

        Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 0.f, 15.f), V_ORIGIN);
        Camera::GetInstance()->SetCameraType(CameraType::FREE_LOOK);
        s.Load();
}
