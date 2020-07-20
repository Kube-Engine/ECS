/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Component
 */

#include <gtest/gtest.h>

#include <Kube/ECS/Component.hpp>

using namespace kF;

TEST(Component, Basics)
{
    struct Data { int x = 0; };

    ECS::Component<Data> component;
    auto index = component.add(0, Data { 42 });

    ASSERT_EQ(component.get(index).x, 42);
}
