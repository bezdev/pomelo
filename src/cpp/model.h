#pragma once

#include "glm/glm.hpp"

#include "mesh.h"
#include "material.h"

class Model {
public:
    Model(glm::vec3 position, Mesh* mesh, Material* material);

    Material* GetMaterial() { return m_Material; }
    Mesh* GetMesh() { return m_Mesh; }
private:
    Model();

    glm::vec3 m_Position;
    Mesh* m_Mesh;
    Material* m_Material;
};
