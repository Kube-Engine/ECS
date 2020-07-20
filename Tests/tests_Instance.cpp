/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Instance
 */

#include <gtest/gtest.h>

#include <Kube/ECS/Instance.hpp>

using namespace kF;

TEST(Instance, Basics)
{
    struct Data { int x = 0; };

    ECS::Instance instance;
    auto dataId = instance.registerComponent<Data>();
    auto entity = instance.registerEntity<Data>();

    ASSERT_NE(entity.index, ECS::NullEntityIndex);
    ASSERT_NE(entity.type, ECS::NullEntityType);
}
