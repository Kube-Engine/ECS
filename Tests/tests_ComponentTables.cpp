/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of ComponentTables
 */

#include <gtest/gtest.h>

#include <Kube/ECS/ComponentTables.hpp>

using namespace kF;

TEST(ComponentTables, Basics)
{
    ECS::ComponentTables<ECS::Entity> table;
    const auto &ctable = table;

    table.add<int>();
    auto &intTable = table.getTable<int>();
    const auto &cintTable = ctable.getTable<int>();
    intTable.add(42, 24);
    ASSERT_EQ(cintTable.get(42), 24);
}
