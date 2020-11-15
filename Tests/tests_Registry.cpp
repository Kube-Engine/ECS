/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Registry
 */

#include <gtest/gtest.h>
#include <math.h>

#include <Kube/ECS/Registry.hpp>
#include <Kube/Flow/Scheduler.hpp>

using namespace kF;

using TypeID = std::type_index;
using Dependencies = std::vector<TypeID>;

struct Position
{
    float x;
    float y;
};

struct Velocity
{
    float dx;
    float dy;
};

struct Wind
{
    float dx;
    float dy;
};

template<ECS::EntityRequirements EntityType>
class SystemInput : public ECS::ASystem<EntityType>
{
public:
    using ECS::ASystem<EntityType>::graph;

    SystemInput(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemInput)) {};
    virtual ~SystemInput(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override
    {
        graph().emplace([&registry, this]() {
            auto view = registry.view<Position, Velocity>();
            view.traverse([this](Position &position, Velocity &velocity) {
                this->update(position, velocity);
            });
        } );
    }

    virtual Dependencies dependencies(void) override
    {
        return {};
    }

private:
    void update(Position &position, Velocity &velocity)
    {
        position.x += velocity.dx;
        position.y += velocity.dy;
    }
};

template<ECS::EntityRequirements EntityType>
class SystemWind : public ECS::ASystem<EntityType>
{
public:
    using ECS::ASystem<EntityType>::graph;

    SystemWind(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemWind)) {};
    virtual ~SystemWind(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override
    {
        graph().emplace([&registry, this] {
            auto view = registry.view<Position, Wind>();
            view.traverse([this](Position &position, const Wind &wind) {
                this->update(position, wind);
            });
        } );
    }

    virtual Dependencies dependencies(void) override
    {
        return { typeid(SystemInput<EntityType>) };
    }

private:
    void update(Position &position, Wind &wind)
    {
        position.x += wind.dx;
        position.y += wind.dy;
    }
};

#define PREPARE_REGISTRY(registry, nbrEntity) \
registry.registerComponent<Position>(); \
registry.registerComponent<Velocity>(); \
for (auto i = 1; i <= nbrEntity; ++i) { \
    auto entity = registry.add(); \
    registry.attach<Position>(entity, i * 1.f, i * 1.f); \
    if (i % 2 == 0) \
        registry.attach<Velocity>(entity, i * .1f, i * .1f); \
}

TEST(Registry, Basics)
{
    ECS::Registry<ECS::Entity> registry;
    const auto entity = registry.add();

#if KUBE_DEBUG_BUILD
    ASSERT_THROW(registry.attach<int>(entity, 42), std::logic_error);
#endif

    registry.registerComponent<int>();
    ASSERT_EQ(registry.attach<int>(entity, 42), 42);
}

TEST(Registry, View)
{
    ECS::Registry<ECS::Entity> registry;

    PREPARE_REGISTRY(registry, 10);

    auto view = registry.view<Position, Velocity>();
    view.traverse([](auto &position, const auto &velocity) {
        position.x += velocity.dx;
        position.y += velocity.dy;
    } );

    const auto &componentTables = registry.componentTables();
    const auto &positionTable = componentTables.getTable<Position>();

    int i = 1;
    for (auto it = positionTable.cbegin(); it != positionTable.cend(); it++) {
        ASSERT_EQ(i + i * .1f * ((i + 1) % 2), (*it).x);
        ASSERT_EQ(i + i * .1f * ((i + 1) % 2), (*it).y);
        i += 1;
    }
    ASSERT_EQ(i - 1, 10);
}

TEST(Registry, SystemGraph)
{
    ECS::Registry<ECS::Entity> registry;

    PREPARE_REGISTRY(registry, 10);
    registry.registerComponent<Wind>();
    for (auto i = 1; i <= 10; ++i) {
        registry.attach<Wind>(i, i * -0.2f, 0.f);
    }

    auto &systemGraph = registry.systemGraph();
    systemGraph.add<SystemWind<ECS::Entity>>();
    systemGraph.add<SystemInput<ECS::Entity>>();
    registry.buildSystemGraph();

    Flow::Scheduler scheduler;
    scheduler.schedule(registry);
    systemGraph.graph().wait();

    const auto &componentTables = registry.componentTables();
    const auto &positionTable = componentTables.getTable<Position>();

    int i = 1;
    for (auto it = positionTable.cbegin(); it != positionTable.cend(); it++) {
//        std::cout << i << " (" << (*it).x << ", " << (*it).y << ")" << std::endl;
//        ASSERT_EQ(i + i * .1f * ((i + 1) % 2), (*it).x);
//        ASSERT_EQ(i + i * .1f * ((i + 1) % 2), (*it).y);
        i += 1;
    }
    ASSERT_EQ(i - 1, 10);
}
