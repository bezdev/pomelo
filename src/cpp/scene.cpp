#include "scene.h"

#include "PhysicsEngine.h"

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
