/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Registry
 */

#include <iostream>
#include <typeindex>
#include <gtest/gtest.h>

#include <Kube/ECS/Registry.hpp>
#include <Kube/Flow/Scheduler.hpp>

using namespace kF;

using TypeID = std::type_index;
using Dependencies = Core::Vector<TypeID>;

template<ECS::EntityRequirements EntityType>
class SystemDummy : public ECS::ASystem<EntityType>
{
public:
    SystemDummy(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemDummy)) {};
    virtual ~SystemDummy(void) override = default;

    void setup(ECS::Registry<ECS::Entity> &registry) {};
    virtual Dependencies dependencies(void) { return Dependencies {}; };
};

template<ECS::EntityRequirements EntityType>
class SystemDummy2 : public ECS::ASystem<EntityType>
{
public:
    SystemDummy2(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemDummy2)) {};
    virtual ~SystemDummy2(void) override = default;

    void setup(ECS::Registry<ECS::Entity> &registry) {};
    virtual Dependencies dependencies(void) { return Dependencies { typeid(SystemDummy<EntityType>) }; };
};

TEST(SystemGraph, Add)
{
    ECS::SystemGraph<ECS::Entity> systemGraph;

    ASSERT_EQ(systemGraph.exists<SystemDummy<ECS::Entity>>(), false);
    systemGraph.add<SystemDummy<ECS::Entity>>();
    ASSERT_EQ(systemGraph.exists<SystemDummy<ECS::Entity>>(), true);
}

TEST(SystemGraph, Get)
{
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<SystemDummy<ECS::Entity>>();
    ASSERT_EQ(systemGraph.exists<SystemDummy<ECS::Entity>>(), true);
    const SystemDummy<ECS::Entity> systemDummy = systemGraph.get<SystemDummy<ECS::Entity>>();
    ASSERT_EQ(systemDummy.typeID().name(), typeid(SystemDummy<ECS::Entity>).name());
}

TEST(SystemGraph, Clear)
{
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<SystemDummy<ECS::Entity>>();
    systemGraph.add<SystemDummy2<ECS::Entity>>();
    ASSERT_EQ(systemGraph.exists<SystemDummy<ECS::Entity>>(), true);
    ASSERT_EQ(systemGraph.exists<SystemDummy2<ECS::Entity>>(), true);
    systemGraph.clear();
    ASSERT_EQ(systemGraph.exists<SystemDummy<ECS::Entity>>(), false);
    ASSERT_EQ(systemGraph.exists<SystemDummy2<ECS::Entity>>(), false);
}

template<ECS::EntityRequirements EntityType, char Character, typename ...DependentSystems>
class SystemSimple : public ECS::ASystem<EntityType>
{
public:
    SystemSimple(std::vector<char> &output) noexcept
        : ECS::ASystem<EntityType>(typeid(SystemSimple)), _output(&output) {};
    virtual ~SystemSimple(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override
    {
        ECS::ASystem<EntityType>::graph().emplace([this] {
            _output->push_back(Character);
        });
    }

    virtual Dependencies dependencies(void) { return Dependencies { typeid(DependentSystems)... }; };
private:
    std::vector<char> *_output;

};


TEST(SystemGraph, SimpleBuild)
{
    using SystemSimpleA = SystemSimple<ECS::Entity, 'A'>;
    using SystemSimpleB = SystemSimple<ECS::Entity, 'B', SystemSimpleA>;

    Flow::Scheduler scheduler;
    ECS::Registry<ECS::Entity> registry;
    std::vector<char> output;

    registry.systemGraph().add<SystemSimpleB>(output);
    registry.systemGraph().add<SystemSimpleA>(output);
    registry.buildSystemGraph();

    for (auto i = 0; i < 1000; ++i) {
        scheduler.schedule(registry);
        registry.systemGraph().graph().wait();
        ASSERT_EQ(output[0], 'A');
        ASSERT_EQ(output[1], 'B');
        output.clear();
    }
}

// TEST(SystemGraph, ComplexBuild)
// {
//     ECS::Registry<ECS::Entity> registry;

//     registry.systemGraph().add<SystemA<ECS::Entity>>();
//     registry.systemGraph().add<SystemB<ECS::Entity>>();
//     registry.systemGraph().add<SystemC<ECS::Entity>>();
//     registry.systemGraph().add<SystemD<ECS::Entity>>();
//     registry.systemGraph().add<SystemE<ECS::Entity>>();
//     registry.buildSystemGraph();

//     // Need to access _systems to check if valid
// }
