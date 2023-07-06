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
    LOGT("ECS");

    ECS ecs;

    auto& e1 = ecs.CreateEntity();
    e1.AddComponent<PositionComponent>(1.f, 2.f);
    e1.AddComponent<VelocityComponent>(0.5, 0.2);

    auto pc = e1.GetComponent<PositionComponent>();
    auto vc = e1.GetComponent<VelocityComponent>();

    ASSERT_ARE_EQUAL(1.f, pc.x);
    ASSERT_ARE_EQUAL(2.f, pc.y);
    ASSERT_ARE_EQUAL(.5f, vc.x);
    ASSERT_ARE_EQUAL(.2f, vc.y);

    ASSERT_TRUE(e1.HasComponent<PositionComponent>());
    ASSERT_TRUE(e1.HasComponent<VelocityComponent>());
    ASSERT_TRUE((e1.HasComponents<PositionComponent, VelocityComponent>()));

    ASSERT_ARE_EQUAL(1, (ecs.GetEntitiesWithComponents<PositionComponent, VelocityComponent>().size()));
}

TEST(NoComponentTest)
{
    ECS ecs;
    auto& e1 = ecs.CreateEntity();
    e1.AddComponent<PositionComponent>(1.f, 2.f);

    ASSERT_TRUE(!e1.HasComponent<VelocityComponent>());
}

TEST(ComponentPointerTest)
{
    ECS ecs;
    auto& e1 = ecs.CreateEntity();
    e1.AddComponent<PositionComponent>(1.f, 2.f);

    PositionComponent* p = nullptr;
    if (e1.HasComponent<PositionComponent>()) {
        p = &e1.GetComponent<PositionComponent>();
    }
    
    ASSERT_ARE_EQUAL(1.f, p->x);
    ASSERT_ARE_EQUAL(2.f, p->y);
}
