#pragma once

#include "ECS.h"
#include "renderer.h"

// TODO: change to enum
#define SCENE_CUBE 1
#define SCENE_MANY_CUBE 2

class Scene
{
public:
    Scene();
    void Load();

    Entity& CreateEntity() { return ECS::GetInstance()->CreateEntity(); };
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
            s.CreateEntity()
                .AddComponent<Components::Motion>(glm::vec3(0, 0, 0))
                .AddComponent<Components::Mesh>(Components::MeshType::BOX)
                .AddComponent<Components::Material>(Components::MaterialType::SolidColor, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
        }

        s.Load();
    }

    static void CreateManyCubeScene()
    {
        Scene s;

        int NUM_BOXES = 100000;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            glm::vec3 p(RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50));

            s.CreateEntity()
                .AddComponent<Components::Motion>(p)
                .AddComponent<Components::Mesh>(Components::MeshType::BOX)
                .AddComponent<Components::Material>(Components::MaterialType::SolidColor, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
        }

        s.Load();
    }
};
