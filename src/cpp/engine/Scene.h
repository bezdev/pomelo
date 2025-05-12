#pragma once

#include "engine/Camera.h"
#include "engine/ECS.h"
#include "render/Text.h"
#include "util/Color.h"
#include "util/Math.h"
#include "util/Timer.h"

enum class SceneType
{
    SCENE_SANDBOX = 0,
    SCENE_GAME,
    SCENE_CUBE,
    SCENE_MANY_CUBE,
    SCENE_MANY_CUBE_INSTANCED,
    SCENE_MANY_CUBE_AXIS,
    SCENE_COLLISION,
    SCENE_COLLISION_NO_PHYSICS,
    SCENE_COLLISION_NO_COLLISION,
    SCENE_JOLT_HELLO_WORLD,
};

class Scene
{
  public:
    Scene();
    void Load();

    Entity &CreateEntity()
    {
        return ECS::GetInstance()->CreateEntity();
    };
};

class SceneManager
{
  public:
    static void LoadScene(SceneType type)
    {
        // type = SceneType::SCENE_COLLISION;
        // type = SceneType::SCENE_SANDBOX;
        // type = SceneType::SCENE_GAME;
        // type = SceneType::SCENE_MANY_CUBE_AXIS;
        // type = SceneType::SCENE_MANY_CUBE_INSTANCED;
        type = SceneType::SCENE_JOLT_HELLO_WORLD;
        // type = SceneType::SCENE_MANY_CUBE_INSTANCED;
        LOGE("LoadScene: %d", static_cast<int>(type));

        if (type == SceneType::SCENE_SANDBOX)
            CreateSandboxScene();
        else if (type == SceneType::SCENE_GAME)
            CreateGameScene();
        else if (type == SceneType::SCENE_CUBE)
            CreateCubeScene();
        else if (type == SceneType::SCENE_MANY_CUBE)
            CreateManyCubeScene();
        else if (type == SceneType::SCENE_MANY_CUBE_INSTANCED)
            CreateManyCubeSceneInstanced();
        else if (type == SceneType::SCENE_MANY_CUBE_AXIS)
            CreateManyCubeAxisScene();
        else if (type == SceneType::SCENE_COLLISION)
            CreateCollisionScene(true, true);
        else if (type == SceneType::SCENE_COLLISION_NO_PHYSICS)
            CreateCollisionScene(false, false);
        else if (type == SceneType::SCENE_COLLISION_NO_COLLISION)
            CreateCollisionScene(true, false);
        else if (type == SceneType::SCENE_JOLT_HELLO_WORLD)
            CreateJoltHelloWorldScene();
    }

    static void CreateSandboxScene();
    static void CreateGameScene();
    static void CreateCollisionScene(bool hasPhysics, bool hasCollisions);
    static void CreateJoltHelloWorldScene();

    static void CreateCubeScene()
    {
        Scene s;

        int NUM_BOXES = 1;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            auto e1 = CREATE_ENTITY();
            ADD_COMPONENT(e1, Components::Transform, glm::vec3(0, 0, 0));
            ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::BOX);
            ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR,
                          glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
        }

        Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 0.f, 0.f));
        s.Load();
    }

    static void CreateManyCubeScene()
    {
        Scene s;

        int NUM_BOXES = 100000;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            glm::vec3 p(RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50));

            auto e1 = CREATE_ENTITY();
            ADD_COMPONENT(e1, Components::Transform, p);
            ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::BOX);
            ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR,
                          glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
        }

        Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 0.f, 0.f));
        s.Load();
    }

    static void CreateManyCubeSceneInstanced()
    {
        Scene s;

        int NUM_BOXES = 100000;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            glm::vec3 p(RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50));

            auto e1 = CREATE_ENTITY();
            ADD_COMPONENT(e1, Components::Transform, p);
            ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::INSTANCED_BOX);
            ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR,
                          glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
        }

        Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 0.f, 0.f));
        s.Load();
    }

    static void CreateManyCubeAxisScene()
    {
        Scene s;

        ScopeTimer t("create ent");
        int NUM_BOXES = 100000;
        for (int i = 0; i < NUM_BOXES; i++)
        {
            glm::vec3 p(RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50));

            auto e1 = CREATE_ENTITY();
            ADD_COMPONENT(e1, Components::Transform, p);
            ADD_COMPONENT(e1, Components::Mesh, Components::MeshType::BOX);
            ADD_COMPONENT(e1, Components::Material, Components::MaterialType::SOLID_COLOR,
                          glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f));
            auto e2 = CREATE_ENTITY();
            ADD_COMPONENT(e2, Components::Transform, p);
            ADD_COMPONENT(e2, Components::Mesh, Components::MeshType::AXIS);
            ADD_COMPONENT(e2, Components::Material, Components::MaterialType::PIXEL_COLOR);
        }

        Camera::GetInstance()->SetLookAt(glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 0.f, 0.f));
        s.Load();
    }
};
