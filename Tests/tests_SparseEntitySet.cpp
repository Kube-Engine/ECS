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
    ECS::Entity entity = 42;

    const auto entityIndex = entities.add(entity);
}
