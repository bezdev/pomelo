#include "Test.h"

#include "engine/CollisionEngine.h"
#include "engine/ECS.h"
#include "util/Logger.h"
#include "util/Timer.h"

std::vector<Entity*> CreateEntities()
{
    std::vector<Entity*> result;

    auto e = ECS::GetInstance();

    int count = 1e5;
    for (int i = 0; i < count; i++)
    {
        glm::vec3 p(RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50), RANDOM_FLOAT(-50, 50));
        result.push_back(&e->CreateEntity()
            .AddComponent<Components::Transform>(p)
            .AddComponent<Components::Mesh>(Components::MeshType::SPHERE)
            .AddComponent<Components::Material>(Components::MaterialType::SOLID_COLOR, glm::vec4(0.2f, 0.709803922f, 0.898039216f, 1.0f)));
        result.push_back(&e->CreateEntity()
            .AddComponent<Components::Transform>(p)
            .AddComponent<Components::Mesh>(Components::MeshType::SPHERE)
            .AddComponent<Components::Material>(Components::MaterialType::PIXEL_COLOR));
    }

    return result;
}

TEST(OctreeTest)
{
    auto es = CreateEntities();
    Octree ot;

    for (auto e : es)
    {
        ot.AddEntity(e);
    }
}

// TEST_THROW()
