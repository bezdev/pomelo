#pragma once

#include <vector>
#include <map>

#include "model.h"

typedef std::vector<Model*> Models;
typedef std::map<MeshType, Models> ModelsByMeshType;
typedef std::map<MaterialType, ModelsByMeshType> ModelsByMaterialByMeshType;
typedef ModelsByMaterialByMeshType RenderObjects;

class RenderObjectCollection {
public:
    RenderObjectCollection();
    ~RenderObjectCollection();

    void Add(Model* model);
    RenderObjects& GetRenderObjects() { return m_RenderObjects; };
private:
    RenderObjects m_RenderObjects;
};
