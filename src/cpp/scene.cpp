#include "scene.h"

Scene::Scene() {

}

void Scene::Load()
{
    Renderer::GetInstance()->LoadEntities(m_ECS.GetEntities());
}

void Scene::AddModel(Model* model) {
   Renderer::GetInstance()->AddRenderObject(model);
}
