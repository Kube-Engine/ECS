/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Registry
 */

#include <gtest/gtest.h>

#include <Kube/ECS/Registry.hpp>

using namespace kF;

TEST(Registry, Basics)
{
    ECS::Registry<ECS::Entity> registry;
    registry.registerComponent<int>();
    const auto entity = registry.add();

    ASSERT_EQ(registry.attach<int>(entity, 42), 42);
}
