/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Registry
 */

#include <iostream>
#include <typeindex>
#include <gtest/gtest.h>

#include <Kube/ECS/Registry.hpp>

using namespace kF;

template<ECS::EntityRequirements EntityType>
class SystemA;

template<ECS::EntityRequirements EntityType>
class SystemB;

template<ECS::EntityRequirements EntityType>
class SystemC;

template<ECS::EntityRequirements EntityType>
class SystemD;

template<ECS::EntityRequirements EntityType>
class SystemE;


using TypeID = std::type_index;
using Dependencies = Core::Vector<TypeID>;

template<ECS::EntityRequirements EntityType>
class SystemA : public ECS::ASystem<EntityType>
{
public:
    SystemA(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemA)) {};
    virtual ~SystemA(void) override = default;

    void setup(ECS::Registry<ECS::Entity> &registry) {};
    virtual Dependencies dependencies(void) { return Dependencies {typeid(SystemB<EntityType>), typeid(SystemE<EntityType>)}; };
};

template<ECS::EntityRequirements EntityType>
class SystemB : public ECS::ASystem<EntityType>
{
public:
    SystemB(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemB)) {};
    virtual ~SystemB(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) {};
    virtual Dependencies dependencies(void) { return Dependencies {}; }
};

template<ECS::EntityRequirements EntityType>
class SystemC : public ECS::ASystem<EntityType>
{
public:
    SystemC(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemC)) {};
    virtual ~SystemC(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) {};
    virtual Dependencies dependencies(void) { return Dependencies {typeid(SystemB<EntityType>), typeid(SystemD<EntityType>)}; }
};

template<ECS::EntityRequirements EntityType>
class SystemD : public ECS::ASystem<EntityType>
{
public:
    SystemD(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemD)) {};
    virtual ~SystemD(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) {};
    virtual Dependencies dependencies(void) { return Dependencies {typeid(SystemB<EntityType>)}; }
};

template<ECS::EntityRequirements EntityType>
class SystemE : public ECS::ASystem<EntityType>
{
public:
    SystemE(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemE)) {};
    virtual ~SystemE(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) {};
    virtual Dependencies dependencies(void) { return Dependencies {typeid(SystemD<EntityType>), typeid(SystemC<EntityType>)}; }
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

TEST(SystemGraph, Clear)
{
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<SystemA<ECS::Entity>>();
    systemGraph.add<SystemB<ECS::Entity>>();
    ASSERT_EQ(systemGraph.exists<SystemA<ECS::Entity>>(), true);
    ASSERT_EQ(systemGraph.exists<SystemB<ECS::Entity>>(), true);
    systemGraph.clear();
    ASSERT_EQ(systemGraph.exists<SystemA<ECS::Entity>>(), false);
    ASSERT_EQ(systemGraph.exists<SystemB<ECS::Entity>>(), false);
}

TEST(SystemGraph, SimpleBuild)
{
    ECS::Registry<ECS::Entity> registry;
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<SystemD<ECS::Entity>>();
    systemGraph.add<SystemB<ECS::Entity>>();
    systemGraph.build(registry);

    // Need to access _systems to check if valid
}

TEST(SystemGraph, ComplexBuild)
{
    ECS::Registry<ECS::Entity> registry;
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<SystemA<ECS::Entity>>();
    systemGraph.add<SystemB<ECS::Entity>>();
    systemGraph.add<SystemC<ECS::Entity>>();
    systemGraph.add<SystemD<ECS::Entity>>();
    systemGraph.add<SystemE<ECS::Entity>>();
    systemGraph.build(registry);

    // Need to access _systems to check if valid
}
