/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of EntityHolder
 */

#include <gtest/gtest.h>

#include <Kube/ECS/EntityHolder.hpp>

using namespace kF;

struct InstanceHolder
{
    InstanceHolder(void) { ECS::Instance::GetRef() = &instance; }
    ~InstanceHolder(void) { ECS::Instance::GetRef() = nullptr; }

    ECS::Instance instance;
};

TEST(EntityHolder, Basics)
{
    struct Data1 { int x = 42; };
    struct Data2 { float y = 12.25f; };
    struct Data3 { char z = 'c'; };

    InstanceHolder instance;
    {
        ECS::EntityHolder<Data1, Data2, Data3> holder;

        ASSERT_EQ(instance.instance.getComponent<Data1>().size(), 1);
        ASSERT_EQ(instance.instance.getComponent<Data2>().size(), 1);
        ASSERT_EQ(instance.instance.getComponent<Data3>().size(), 1);
        ASSERT_EQ(holder.get<Data1>().x, 42);
        ASSERT_EQ(holder.get<Data2>().y, 12.25f);
        ASSERT_EQ(holder.get<Data3>().z, 'c');
    }
    ASSERT_EQ(instance.instance.getComponent<Data1>().size(), 0);
    ASSERT_EQ(instance.instance.getComponent<Data2>().size(), 0);
    ASSERT_EQ(instance.instance.getComponent<Data3>().size(), 0);
}
