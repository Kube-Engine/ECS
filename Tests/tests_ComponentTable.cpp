/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of ComponentTable
 */

#include <gtest/gtest.h>

#include <Kube/ECS/ComponentTable.hpp>

using namespace kF;

TEST(ComponentTable, Basics)
{
    ECS::ComponentTable<int, ECS::Entity> table;
    ECS::Entity entity = 42;

    ASSERT_EQ(table.add(entity, 24), 24);
}
