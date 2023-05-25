#include "Test.h"

#include "ECS.h"
#include "logger.h"

struct PositionComponent {
    float x;
    float y;

    PositionComponent():
        x(0),
        y(0)
    {}

    PositionComponent(float x, float y):
        x(x),
        y(y)
    {}
};

struct VelocityComponent {
    float x;
    float y;

    VelocityComponent():
        x(0),
        y(0)
    {}

    VelocityComponent(float x, float y):
        x(x),
        y(y)
    {}
};

TEST(BasicECSTest)
{
    LOGT("LEO");

    ECS ecs;

    auto& entity1 = ecs.CreateEntity();
    entity1.AddComponent<PositionComponent>(1.f, 2.f);
    entity1.AddComponent<VelocityComponent>(0.5, 0.2);

    std::cout << "Entity 1 position: (" << entity1.GetComponent<PositionComponent>().x
              << ", " << entity1.GetComponent<PositionComponent>().y << ")" << std::endl;

    // Check if an entity has a component
    if (entity1.HasComponent<VelocityComponent>()) {
        std::cout << "Entity 1 has velocity component" << std::endl;
    }
    // Ecs ecs;

    // ecs.RegisterComponent<PositionComponent>();
    // ecs.RegisterComponent<VelocityComponent>();

    // Entity entity = ecs.CreateEntity();
    // ecs.AddComponent(entity, PositionComponent{1.0f, 2.0f});
    // ecs.AddComponent(entity, VelocityComponent{3.0f, 4.0f});

    // auto position = ecs.GetComponent<PositionComponent>(entity);
    // auto velocity = ecs.GetComponent<VelocityComponent>(entity);

    // std::vector<Entity> entities = ecs.GetEntitiesWithComponents<PositionComponent, VelocityComponent>();
    // LOGT("entity count: %d", entities.size());

    // const std::size_t N = 10;
    // ObjectPool<ClassA, N> op;
    // op.Create();
    // ASSERT_ARE_EQUAL(op.Count(), 1);

    // for (std::size_t i = 1; i < N; ++i)
    // {
    //     op.Create("line", i);
    // }

    // ASSERT_ARE_EQUAL(op.Count(), 10);

    // ASSERT_THROWS(op.Create());

    // LOGT("sizeof(ClassA): %d", sizeof(ClassA));
    // LOGT("sizeof(op): %d", sizeof(op));
}
