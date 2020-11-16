/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of View
 */

#include <gtest/gtest.h>

#include <Kube/ECS/View.hpp>

using namespace kF;

TEST(View, BasicsTraverse)
{
    ECS::ComponentTable<int, ECS::Entity> table;
    ECS::View<ECS::Entity, int> view(table);
    ECS::Entity entity = 42;

    ASSERT_EQ(view.traverse([](int){}), false);

    table.add(entity, 42);

    ASSERT_EQ(view.traverse([](int value) {
        ASSERT_EQ(value, 42);
    }), true);
}

TEST(View, MultipleTraverse)
{
    ECS::ComponentTable<int, ECS::Entity> table1;
    ECS::ComponentTable<float, ECS::Entity> table2;
    ECS::ComponentTable<bool, ECS::Entity> table3;
    ECS::View<ECS::Entity, int, float, bool> view(table1, table2, table3);

    for (int i = 0; i < 42; i += 1) {
        table1.add(i, i);
        table2.add(i, i * 2);
        table3.add(i, i % 2);
    }

    int i = 0;
    ASSERT_EQ(view.traverse([&i](int value1, float value2, bool value3) {
        ASSERT_EQ(value1, i);
        ASSERT_EQ(value2, i * 2);
        ASSERT_EQ(value3, i % 2);
        i += 1;
    }), true);

    ASSERT_EQ(i, 42);
}

TEST(View, BasicsCollect)
{
    ECS::ComponentTable<int, ECS::Entity> table;
    ECS::View<ECS::Entity, int> view(table);
    ECS::Entity entity = 42;
    Core::Vector<ECS::Entity> entities;

    view.collect(entities);
    ASSERT_EQ(entities.size(), 0);

    table.add(entity, 42);

    view.collect(entities);
    ASSERT_EQ(entities.size(), 1);
    ASSERT_EQ(entities[0], entity);
}

TEST(View, MultipleCollect)
{
    ECS::ComponentTable<int, ECS::Entity> table1;
    ECS::ComponentTable<float, ECS::Entity> table2;
    ECS::ComponentTable<bool, ECS::Entity> table3;
    ECS::View<ECS::Entity, int, float, bool> view(table1, table2, table3);
    Core::Vector<ECS::Entity> entities;

    for (int i = 0; i < 42; i += 1) {
        table1.add(i, i);
        table2.add(i, i * 2);
        if (i % 2 == 0)
            table3.add(i, i % 2);
    }

    view.collect(entities);
    ASSERT_EQ(entities.size(), 21);
    for (int i = 0; i < 21; i += 1)
        ASSERT_EQ(entities[i], i * 2);
}
