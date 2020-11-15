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
using Dependencies = std::vector<TypeID>;

template<ECS::EntityRequirements EntityType>
class DummySystem : public ECS::ASystem<EntityType>
{
public:
    DummySystem() noexcept : ECS::ASystem<EntityType>(typeid(DummySystem)) {};
    virtual ~DummySystem(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override {}

    virtual Dependencies dependencies(void) { return Dependencies {}; };
};


template<ECS::EntityRequirements EntityType, char Character, typename ...DependentSystems>
class DependentSystem : public ECS::ASystem<EntityType>
{
public:
    DependentSystem() noexcept : ECS::ASystem<EntityType>(typeid(DependentSystem)) {};
    DependentSystem(std::vector<char> &output) noexcept
        : ECS::ASystem<EntityType>(typeid(DependentSystem)), _output(&output) {};
    virtual ~DependentSystem(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override
    {
        if (_output)
            ECS::ASystem<EntityType>::graph().emplace([this] { _output->push_back(Character); });
    }

    virtual Dependencies dependencies(void) { return Dependencies { typeid(DependentSystems)... }; };

private:
    std::vector<char> *_output;
};

TEST(SystemGraph, Add)
{
    ECS::SystemGraph<ECS::Entity> systemGraph;

    ASSERT_EQ(systemGraph.exists<DummySystem<ECS::Entity>>(), false);
    systemGraph.add<DummySystem<ECS::Entity>>();
    ASSERT_EQ(systemGraph.exists<DummySystem<ECS::Entity>>(), true);
}

TEST(SystemGraph, Get)
{
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<DummySystem<ECS::Entity>>();
    ASSERT_EQ(systemGraph.exists<DummySystem<ECS::Entity>>(), true);
    const DummySystem<ECS::Entity> systemDummy = systemGraph.get<DummySystem<ECS::Entity>>();
    ASSERT_EQ(systemDummy.typeID().name(), typeid(DummySystem<ECS::Entity>).name());
}

TEST(SystemGraph, Clear)
{
    using DependentSystemA = DependentSystem<ECS::Entity, 'A'>;
    using DependentSystemB = DependentSystem<ECS::Entity, 'B'>;

    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<DependentSystemA>();
    systemGraph.add<DependentSystemB>();
    ASSERT_EQ(systemGraph.exists<DependentSystemA>(), true);
    ASSERT_EQ(systemGraph.exists<DependentSystemB>(), true);
    systemGraph.clear();
    ASSERT_EQ(systemGraph.exists<DependentSystemA>(), false);
    ASSERT_EQ(systemGraph.exists<DependentSystemB>(), false);
}

TEST(SystemGraph, SimpleBuild)
{
    using DependentSystemA = DependentSystem<ECS::Entity, 'A'>;
    using DependentSystemB = DependentSystem<ECS::Entity, 'B', DependentSystemA>;

    Flow::Scheduler scheduler;
    ECS::Registry<ECS::Entity> registry;
    std::vector<char> output;

    registry.systemGraph().add<DependentSystemB>(output);
    registry.systemGraph().add<DependentSystemA>(output);
    registry.buildSystemGraph();

    scheduler.schedule(registry);
    registry.systemGraph().graph().wait();
    ASSERT_EQ(output[0], 'A');
    ASSERT_EQ(output[1], 'B');
    output.clear();
}

TEST(SystemGraph, ComplexBuild)
{
    using DependentSystemB = DependentSystem<ECS::Entity, 'B'>;
    using DependentSystemD = DependentSystem<ECS::Entity, 'D', DependentSystemB>;
    using DependentSystemC = DependentSystem<ECS::Entity, 'C', DependentSystemB, DependentSystemD>;
    using DependentSystemE = DependentSystem<ECS::Entity, 'E', DependentSystemC, DependentSystemD>;
    using DependentSystemA = DependentSystem<ECS::Entity, 'A', DependentSystemB, DependentSystemE>;

    Flow::Scheduler scheduler;
    ECS::Registry<ECS::Entity> registry;
    std::vector<char> output;

    registry.systemGraph().add<DependentSystemA>(output);
    registry.systemGraph().add<DependentSystemB>(output);
    registry.systemGraph().add<DependentSystemC>(output);
    registry.systemGraph().add<DependentSystemD>(output);
    registry.systemGraph().add<DependentSystemE>(output);
    registry.buildSystemGraph();

    scheduler.schedule(registry);
    registry.systemGraph().graph().wait();
    ASSERT_EQ(output[0], 'B');
    ASSERT_EQ(output[1], 'D');
    ASSERT_EQ(output[2], 'C');
    ASSERT_EQ(output[3], 'E');
    ASSERT_EQ(output[4], 'A');
    output.clear();
}


template<ECS::EntityRequirements EntityType>
class CircularSystemA;
template<ECS::EntityRequirements EntityType>
class CircularSystemB;
template<ECS::EntityRequirements EntityType>
class CircularSystemC;

template<ECS::EntityRequirements EntityType>
class CircularSystemA : public ECS::ASystem<EntityType>
{
public:
    CircularSystemA() noexcept : ECS::ASystem<EntityType>(typeid(CircularSystemA)) {};
    virtual ~CircularSystemA(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override {}
    virtual Dependencies dependencies(void) { return Dependencies {typeid(CircularSystemB<EntityType>)}; };
};

template<ECS::EntityRequirements EntityType>
class CircularSystemB : public ECS::ASystem<EntityType>
{
public:
    CircularSystemB() noexcept : ECS::ASystem<EntityType>(typeid(CircularSystemB)) {};
    virtual ~CircularSystemB(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override {}
    virtual Dependencies dependencies(void) { return Dependencies {typeid(CircularSystemC<EntityType>)}; };
};

template<ECS::EntityRequirements EntityType>
class CircularSystemC : public ECS::ASystem<EntityType>
{
public:
    CircularSystemC() noexcept : ECS::ASystem<EntityType>(typeid(CircularSystemC)) {};
    virtual ~CircularSystemC(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override {}
    virtual Dependencies dependencies(void) { return Dependencies {typeid(CircularSystemA<EntityType>)}; };
};

TEST(SystemGraph, CircularDependency)
{
    Flow::Scheduler scheduler;
    ECS::Registry<ECS::Entity> registry;

    registry.systemGraph().add<CircularSystemA<ECS::Entity>>();
    registry.systemGraph().add<CircularSystemB<ECS::Entity>>();
    registry.systemGraph().add<CircularSystemC<ECS::Entity>>();
    ASSERT_THROW(registry.buildSystemGraph(), std::logic_error);
}
