#include "scene.h"

Scene::Scene() {

}

void Scene::AddModel(Model* model) {
    Renderer::GetInstance()->AddRenderObject(model);
}
