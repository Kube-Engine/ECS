/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of SparseEntitySet
 */

#include <gtest/gtest.h>

#include <Kube/ECS/Base.hpp>
#include <Kube/ECS/SparseEntitySet.hpp>

using namespace kF;

TEST(SparseEntitySet, Basics)
{
    constexpr ECS::Entity PageSize = 16384u / sizeof(ECS::Entity);

    ECS::SparseEntitySet<ECS::Entity, PageSize> entities;
    ECS::Entity entity1 = 24;
    ECS::Entity entity2 = 42;

    ASSERT_EQ(entities.exists(entity1), false);

    const auto entityIndex = entities.add(entity1);

    ASSERT_EQ(entities.exists(entity1), true);
    ASSERT_EQ(entities.entityCount(), 1);
    ASSERT_EQ(entities.at(entity1), entityIndex);

    entities.remove(entity1);

    ASSERT_EQ(entities.entityCount(), 0);

    entities.add(entity1);
    entities.add(entity2);

    ASSERT_THROW(entities.add(entity1), std::logic_error);

    entities.clear();

    ASSERT_THROW(entities.remove(entity2), std::logic_error);
}
