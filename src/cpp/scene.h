#pragma once

#include "model.h"
#include "renderer.h"

#define SCENE_CUBE 1

class Scene
{
public:
    Scene();
    void AddModel(Model* model);
};

class SceneManager
{
public:
    static void LoadScene(int sceneId)
    {
        if (sceneId == SCENE_CUBE) CreateCubeScene();
    }

    static void CreateCubeScene()
    {
        Scene s;

        int NUM_BOXES = 1;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            Model *m = new Model(
                    glm::vec3(0, 0, 0),
                    Mesh::PRIMITIVE_BOX_TRIANGLE,
                    Material::CreateSolidColorMaterial(
                            glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f)));

            s.AddModel(m);
        }
    }
};
