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

TEST(ComponentTable, Dispatchers)
{
    ECS::ComponentTable<int, ECS::Entity> table;
    auto &addDispatcher = table.getAddDispatcher();
    auto &removeDispatcher = table.getRemoveDispatcher();
    int nbrAddDispatcherCalled = 0;
    int nbrRemoveDispatcherCalled = 0;

    addDispatcher.add([&nbrAddDispatcherCalled](ECS::Entity entity) {
        ASSERT_EQ(entity, nbrAddDispatcherCalled);
        nbrAddDispatcherCalled += 1;
    } );
    removeDispatcher.add([&nbrRemoveDispatcherCalled](ECS::Entity entity) {
        ASSERT_EQ(entity, nbrRemoveDispatcherCalled);
        nbrRemoveDispatcherCalled += 1;
    } );

    for (int i = 0; i < 100; i += 1)
        table.add(i, i);
    for (int i = 0; i < 50; i += 1)
        table.remove(i);

    ASSERT_EQ(nbrAddDispatcherCalled, 100);
    ASSERT_EQ(nbrRemoveDispatcherCalled, 50);
}
