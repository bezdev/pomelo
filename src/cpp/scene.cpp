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

        glm::vec3 p(0.f, 0.f, 0.f);

        EntityFactory::CreateAxis(p, glm::vec3(0.f, 2.f, 0.f));
        s.CreateEntity()
            .AddComponent<Components::Transform>(p)
            .AddComponent<Components::Mesh>(Components::MeshType::BOX)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(1.0f, 0.f, 0.f, 1.0f))
            .AddComponent<Components::Motion>(Components::MotionType::ORBIT, glm::vec3(-10.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 5000);

        s.CreateEntity()
            .AddComponent<Components::Transform>(p)
            .AddComponent<Components::Mesh>(Components::MeshType::SPHERE)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 0.f, 1.f, 1.0f))
            .AddComponent<Components::Motion>(Components::MotionType::ORBIT, glm::vec3(10.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 5000);

        s.CreateEntity()
            .AddComponent<Components::Transform>(p)
            .AddComponent<Components::Mesh>(Components::MeshType::INSTANCED_BOX)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.0f, 1.f, 0.f, 1.0f));

        Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 0.f, 20.f), glm::vec3(0.f, 0.f, 0.f));
        s.Load();
}
