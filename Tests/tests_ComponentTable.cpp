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

    ASSERT_EQ(table.exists(entity), false);

    const auto &component = table.add(entity, 24);

    ASSERT_EQ(table.exists(entity), true);
    ASSERT_EQ(table.size(), 1);
    ASSERT_EQ(component, 24);
    ASSERT_EQ(&(table.get(entity)), &component);

    table.remove(entity);

    ASSERT_EQ(table.size(), 0);

    table.add(entity);

    ASSERT_THROW(table.add(entity), std::logic_error);

    table.clear();

    ASSERT_THROW(table.remove(entity), std::logic_error);
}
