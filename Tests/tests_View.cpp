/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of View
 */

#include <gtest/gtest.h>

#include <Kube/ECS/View.hpp>

using namespace kF;

TEST(View, Basics)
{
    ECS::ComponentTable<int, ECS::Entity> table;
    ECS::View<ECS::Entity, int> view(table);
    ECS::Entity entity = 42;

    ASSERT_EQ(view.traverse([](int){}), false);

    table.add(entity, 42);

    ASSERT_EQ(view.traverse([](int value) {
        ASSERT_EQ(value, 42);
    }), true);

    return;

    ECS::ComponentTable<float, ECS::Entity> table2;
    ECS::ComponentTable<bool, ECS::Entity> table3;
    ECS::View<ECS::Entity, int, float, bool> view2(table, table2, table3);

    for (int i = 0; i <= 42; i += 1) {
        if (i < 42)
            table.add(i, i);
        table2.add(i, i * 2);
        table3.add(i, i % 2);
    }
    view2.traverse([](int value, float value2, bool value3) {
        std::cout << std::boolalpha << value << " " << value2 << " " << value3 << std::endl;
    });
}
