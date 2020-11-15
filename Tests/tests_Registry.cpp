/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Registry
 */

#include <gtest/gtest.h>

#include <Kube/ECS/Registry.hpp>

using namespace kF;

struct position {
    position(float x, float y) : x(x), y(y) {}
    float x;
    float y;
};

struct velocity {
    velocity(float dx, float dy) : dx(dx), dy(dy) {}
    float dx;
    float dy;
};

TEST(Registry, Basics)
{
    ECS::Registry<ECS::Entity> registry;
    registry.registerComponent<int>();
    const auto entity = registry.add();

    ASSERT_EQ(registry.attach<int>(entity, 42), 42);
}

TEST(Registry, StructComponent)
{
    ECS::Registry<ECS::Entity> registry;

    for (auto i = 0; i < 10; ++i) {
        auto entity = registry.add();

        registry.attach<position>(entity, i * 1.f, i * 1.f);
        if (i % 2 == 0)
            registry.attach<velocity>(entity, i * .1f, i * .1f);
    }

    auto view = registry.view<position, velocity>();
    view.traverse([](auto &position, auto &velocity) {
        position.x += velocity.dx;
        position.y += velocity.dy;
    } );
}
