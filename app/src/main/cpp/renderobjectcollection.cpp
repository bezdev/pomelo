#include "renderobjectcollection.h"

// TODO: replace with object pools
RenderObjectCollection::RenderObjectCollection()
{
    
}

// TODO: cleanup
RenderObjectCollection::~RenderObjectCollection()
{

}

void RenderObjectCollection::Add(Model* model)
{
    MaterialType materialType = model->GetMaterial()->GetType();
    MeshType meshType = model->GetMesh()->GetType();

    /*
    if (!ValidateMaterialAndMeshTypes(materialType, meshType))
    {
        throw std::runtime_error("Material and Mesh types are not compatible.");
    }
    */

    // TODO: this is bad just initialize them

    // Does the material type exist?
    if (m_RenderObjects.find(materialType) == m_RenderObjects.end())
    {
        m_RenderObjects[materialType] = ModelsByMeshType();
    }

    // Does the mesh type exist?
    if (m_RenderObjects[materialType].find(meshType) == m_RenderObjects[materialType].end())
    {
        m_RenderObjects[materialType][meshType] = Models();
    }

    m_RenderObjects[materialType][meshType].push_back(model);
}