/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Registry
 */

#include <iostream>
#include <typeindex>
#include <gtest/gtest.h>

#include <Kube/ECS/Registry.hpp>

using namespace kF;

using TypeID = std::type_index;
using Dependencies = Core::Vector<TypeID>;

template<ECS::EntityRequirements EntityType>
class SystemA : public ECS::ASystem<EntityType>
{
public:
    SystemA(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemA)) {};
    virtual ~SystemA(void) override = default;

    void setup(ECS::Registry<ECS::Entity> &registry) { std::cout << "SETUP SYSTEM A" << std::endl; };

    Dependencies dependencies(void) {
        Core::Vector<TypeID> test;
        return test;
    };
};

template<ECS::EntityRequirements EntityType>
class SystemB : public ECS::ASystem<EntityType>
{
public:
    SystemB(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemB)) {};
    virtual ~SystemB(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) { std::cout << "SETUP SYSTEM B" << std::endl; };

    virtual Dependencies dependencies(void)
    {
        Core::Vector<TypeID> test;
        test.push(typeid(SystemA<EntityType>));
        return test;
    }
};

TEST(SystemGraph, Add)
{
    ECS::SystemGraph<ECS::Entity> systemGraph;

    ASSERT_EQ(systemGraph.exists<SystemA<ECS::Entity>>(), false);
    systemGraph.add<SystemA<ECS::Entity>>();
    ASSERT_EQ(systemGraph.exists<SystemA<ECS::Entity>>(), true);
}

TEST(SystemGraph, Get)
{
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<SystemA<ECS::Entity>>();
    ASSERT_EQ(systemGraph.exists<SystemA<ECS::Entity>>(), true);
    const SystemA<ECS::Entity> systemA = systemGraph.get<SystemA<ECS::Entity>>();
    ASSERT_EQ(systemA.typeID().name(), typeid(SystemA<ECS::Entity>).name());
}

TEST(SystemGraph, Build)
{
    ECS::Registry<ECS::Entity> registry;
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<SystemA<ECS::Entity>>();
    systemGraph.add<SystemB<ECS::Entity>>();
    // systemGraph.build(registry);
}
