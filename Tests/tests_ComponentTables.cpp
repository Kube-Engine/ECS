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

    ASSERT_EQ(ctable.tableExists<int>(), false);

    table.add<int>();

    ASSERT_EQ(ctable.tableExists<int>(), true);
    ASSERT_EQ(ctable.size(), 1);

    auto &intTable = table.getTable<int>();
    const auto &cintTable = ctable.getTable<int>();

    intTable.add(42, 24);
    ASSERT_EQ(cintTable.get(42), 24);

    ASSERT_THROW(table.add<int>(), std::logic_error);
    ASSERT_THROW(table.getTable<float>(), std::logic_error);
}
