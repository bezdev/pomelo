#pragma once

#include "ECS.h"
#include "renderer.h"

// TODO: change to enum
#define SCENE_CUBE 1
#define SCENE_MANY_CUBE 2
#define SCENE_MANY_CUBE_AXIS 3

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
        LOGE("LoadScene: %d", sceneId);
        if (sceneId == SCENE_CUBE) CreateCubeScene();
        else if (sceneId == SCENE_MANY_CUBE) CreateManyCubeScene();
        else if (sceneId == SCENE_MANY_CUBE_AXIS) CreateManyCubeAxisScene();
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
                .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
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
                .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
        }

        s.Load();
    }

    static void CreateManyCubeAxisScene()
    {
        Scene s;

        int NUM_BOXES = 100000;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            glm::vec3 p(RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50));

            s.CreateEntity()
                .AddComponent<Components::Motion>(p)
                .AddComponent<Components::Mesh>(Components::MeshType::BOX)
                .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
            s.CreateEntity()
                .AddComponent<Components::Motion>(p)
                .AddComponent<Components::Mesh>(Components::MeshType::AXIS)
                .AddComponent<Components::Material>(Components::MaterialType::PIXEL_COLOR);
        }

        s.Load();
    }
};
