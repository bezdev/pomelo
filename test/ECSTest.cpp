#include "Test.h"

#include "engine/ECS.h"
#include "util/Logger.h"
#include "vendor/entt/entt.hpp"
#include <util/Timer.h>

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


TEST(BasicECSTest2) {
    LOGT("EnTT ECS");

    entt::registry registry;

    auto entity = registry.create();

    registry.emplace<PositionComponent>(entity, 1.f, 2.f);
    registry.emplace<VelocityComponent>(entity, 0.5f, 0.2f);

    auto& pc = registry.get<PositionComponent>(entity);
    auto& vc = registry.get<VelocityComponent>(entity);

    assert(pc.x == 1.f);
    assert(pc.y == 2.f);
    assert(vc.x == 0.5f);
    assert(vc.y == 0.2f);

    assert(registry.any_of<PositionComponent>(entity));
    assert(registry.any_of<VelocityComponent>(entity));

    assert((registry.any_of<PositionComponent, VelocityComponent>(entity)));

    size_t count = 0;
    registry.view<PositionComponent, VelocityComponent>().each([&count](auto, auto&, auto&) {
        ++count;
        });
    assert(count == 1);
}

TEST(PerfTest)
{
    size_t entityCount = 1e5;
    float oldSum = 0.f;
    float newSum = 0.f;
    
    {
        ScopeTimer t("old");

        ECS ecs;
        for (size_t i = 0; i < entityCount; ++i) {
            auto& entity = ecs.CreateEntity();
            entity.AddComponent<PositionComponent>(static_cast<float>(i), static_cast<float>(i + 1));
            entity.AddComponent<VelocityComponent>(static_cast<float>(i * 0.1), static_cast<float>(i * 0.2));
        }

        for (auto& entity : ecs.GetEntitiesWithComponents<PositionComponent, VelocityComponent>()) {
            auto pos = entity->GetComponent<PositionComponent>();
            auto vel = entity->GetComponent<VelocityComponent>();

            oldSum += pos.x + pos.y + vel.x + vel.y;
        }

        std::cout << "Dummy sum (Old ECS): " << oldSum << '\n';
    }

    {
        std::cout << "Testing EnTT ECS:\n";
        entt::registry registry;

        {
            ScopeTimer t("new");


            Timer timer;
            for (size_t i = 0; i < entityCount; ++i) {
                auto entity = registry.create();
                registry.emplace<PositionComponent>(entity, static_cast<float>(i), static_cast<float>(i + 1));
                registry.emplace<VelocityComponent>(entity, static_cast<float>(i * 0.1), static_cast<float>(i * 0.2));
            }

            auto view = registry.view<PositionComponent, VelocityComponent>();
            view.each([&newSum](const auto& pos, const auto& vel) {
                newSum += pos.x + pos.y + vel.x + vel.y;
            });
        }

        std::cout << "Dummy sum (EnTT ECS): " << newSum << '\n';
    }
}
