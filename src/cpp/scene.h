#pragma once

#include "ECS.h"
#include "model.h"
#include "renderer.h"

// TODO: change to enum
#define SCENE_CUBE 1
#define SCENE_MANY_CUBE 2

class Scene
{
public:
    Scene();
    void Load();
    void AddModel(Model* model);

    Entity& CreateEntity() { return m_ECS.CreateEntity(); };

private:
    ECS m_ECS;
};

class SceneManager
{
public:
    static void LoadScene(int sceneId)
    {
        LOGE("sceneId: %d", sceneId);
        if (sceneId == SCENE_CUBE) CreateCubeScene();
        else if (sceneId == SCENE_MANY_CUBE) CreateManyCubeScene();
    }

    static void CreateCubeScene()
    {
        Scene s;

        int NUM_BOXES = 1;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            // s.CreateEntity()
            //     .AddComponent<Motion>(glm::vec3(0, 0, 0))
            //     .AddComponent<Mesh>()
            //     .AddComponent<Material>();

            Model *m = new Model(
                    glm::vec3(0, 0, 0),
                    Mesh::PRIMITIVE_BOX_TRIANGLE,
                    Material::CreateSolidColorMaterial(
                            glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f)));

            s.AddModel(m);
        }
    }

    static void CreateManyCubeScene()
    {
        Scene s;

        int NUM_BOXES = 100000;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            glm::vec3 p(RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50));

            Model *m = new Model(
                    p,
                    Mesh::PRIMITIVE_BOX_TRIANGLE,
                    Material::CreateSolidColorMaterial(
                            glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f)));

            s.AddModel(m);
        }
    }
};
