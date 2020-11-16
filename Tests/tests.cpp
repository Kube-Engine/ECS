/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of View
 */

#include <gtest/gtest.h>

#include <Kube/ECS/Registry.hpp>
#include <Kube/Flow/Scheduler.hpp>

using namespace kF;

struct Position
{
    int x;
    int y;
};

struct Velocity
{
    int x;
    int y;
};

struct Rigidbody
{
    using CollideDispatcher = Core::TrivialDispatcher<void (Rigidbody &)>;

    int width;
    int height;
    CollideDispatcher collideDispatcher;
};

struct CharacterData
{
    int life;
    int damage;
    int range;
};

template<ECS::EntityRequirements EntityType>
class SystemMovement : public ECS::ASystem<EntityType>
{
public:
    using ECS::ASystem<EntityType>::graph;

    SystemMovement(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemMovement)) {};
    virtual ~SystemMovement(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override
    {
        graph().emplace([&registry, this] {
            registry.view<Position, Velocity>().traverse([this](Position &position, const Velocity &velocity) {
    position.x += velocity.x;
    position.y += velocity.y;
            });
        } );
    }

    virtual ECS::ASystem<EntityType>::Dependencies dependencies(void) override
    {
        return {};
    }
};

template<ECS::EntityRequirements EntityType>
class SystemPhysics : public ECS::ASystem<EntityType>
{
public:
    using ECS::ASystem<EntityType>::Dependencies;
    using ECS::ASystem<EntityType>::graph;

    SystemPhysics(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemPhysics)) {};
    virtual ~SystemPhysics(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override
    {
        auto retrieveEntities = graph().emplace([&registry, this] {
            registry.view<Position, Rigidbody>().collect(_entities);
        } );
    auto checkCollision = graph().emplace([&registry, this] {
            auto &componentTables = registry.componentTables();
            auto &tablePosition = componentTables.getTable<Position>();
            auto &tableRigidbody = componentTables.getTable<Rigidbody>();
            auto end = _entities.cend();

            for (auto it = _entities.cbegin(); it != end--; it++) {
                auto &position1 = tablePosition.get(*it);
                auto &rigidbody1 = tableRigidbody.get(*it);

                for (auto testIt = it++; testIt != end; testIt++) {
                    auto &position2 = tablePosition.get(*testIt);
                    auto &rigidbody2 = tableRigidbody.get(*testIt);

                    if (collide(position1, rigidbody1, position2, rigidbody2)) {
                        rigidbody1.collideDispatcher.dispatch(rigidbody2);
                        rigidbody2.collideDispatcher.dispatch(rigidbody1);
                    }
                }
            }
        } );

        retrieveEntities.precede(checkCollision);
    }

    virtual ECS::ASystem<EntityType>::Dependencies dependencies(void) override
    {
        return {};
    }

private:
    Core::Vector<EntityType> _entities {};

    inline bool collide(const Position &position1, const Rigidbody &rigidbody1, const Position &position2, const Rigidbody &rigidbody2)
    {
        return position1.x < (position2.x + rigidbody2.width) && position1.y > (position2.y + rigidbody2.height)
            && (position1.x + rigidbody1.width) > position2.x && (position1.y + rigidbody1.height) < position2.y;
    }
};

template<ECS::EntityRequirements EntityType>
class SystemAI : public ECS::ASystem<EntityType>
{
public:
    using ECS::ASystem<EntityType>::Dependencies;
    using ECS::ASystem<EntityType>::graph;

    SystemAI(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemAI)) {};
    virtual ~SystemAI(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override
    {
        auto retrieveEntities = graph().emplace([&registry, this] {
            registry.view<Position, CharacterData>().collect(_entities);
        } );
        auto checkRange = graph().emplace([&registry, this] {
            auto &componentTables = registry.componentTables();
            auto &tablePosition = componentTables.getTable<Position>();
            auto &tableCharacterData = componentTables.getTable<CharacterData>();
            auto end = _entities.cend();

            for (auto it = _entities.cbegin(); it != end--; it++) {
                auto &position1 = tablePosition.get(*it);
                auto &characterData1 = tableCharacterData.get(*it);

                for (auto testIt = it++; testIt != end; testIt++) {
                    auto &position2 = tablePosition.get(*testIt);
                    auto &characterData2 = tableCharacterData.get(*testIt);

                    if (characterData1.range <= (position2.x - position1.x) + (position2.y - position1.y)) {
                        characterData2.life -= characterData1.damage;
                    }
                }
            }
        });
    }

    virtual ECS::ASystem<EntityType>::Dependencies dependencies(void) override
    {
        return {};
    }

private:
    Core::Vector<EntityType> _entities {};
};

TEST(ECS, Basics)
{
    ECS::Registry<ECS::Entity> registry;

    registry.registerComponent<Position>();
    registry.registerComponent<Velocity>();
    registry.registerComponent<Rigidbody>();
    registry.registerComponent<CharacterData>();

    auto player = registry.add();
    registry.attach<Position>(player, 0, 0);
    registry.attach<Velocity>(player, 1, 0);
    registry.attach<Rigidbody>(player, 1, 2, Rigidbody::CollideDispatcher());
    registry.attach<CharacterData>(player, 100, 15, 1);

    auto enemy = registry.add();
    registry.attach<Position>(enemy, 10, 0);
    registry.attach<Rigidbody>(enemy, 1, 1);
    registry.attach<CharacterData>(enemy, 75, 10, 2);

    auto &systemGraph = registry.systemGraph();
    systemGraph.add<SystemMovement<ECS::Entity>>();
    systemGraph.add<SystemPhysics<ECS::Entity>>();
    systemGraph.add<SystemAI<ECS::Entity>>();
    registry.buildSystemGraph();

    Flow::Scheduler scheduler;
    scheduler.schedule(registry);
    systemGraph.graph().wait();
}

/*

Components:
Position (x<int>, y<int>)
Velocity (x<int>, y<int>)
RigidBody (isCollide<bool>)
Player (life<int>)
Skeleton (life<int>, range<int>, isTargetAcquired<bool>, target<Mob *>)
Arrow (damage<int>)

Systems:
Movement
    updatePosition<Position, Velocity, RigidBody> # if RigidBody's isCollide is true, do not update
    isCollide<Position, Velocity, RigidBody> # if actual pos + velocity is going to share coordinate with any other's pos, set isCollide to true.

View
    isView<Skeleton, Player> # if player's pos < skeleton's pos + skeleton's viewRay, skeleton's isTargetAcquired is set to true and a pointer to players is stocked in skeleton's target

AI
    skeletonAI<Skeleton> # if skeleton's isTargetAcquired is True, generate a new arrow entity and set her velocity towards target's pos
    PlayerAI<Player> # if player's isCollide is True and is collided by an arrow, set life to life - arrow's damage and destroy the arrow. !!!! WE NEED TO FIGURE OUT HOW TO DETERMINE 'and is collided by an arrow' !!!!

System's Priorities:
Movement (Sequential)
isCollide
    updatePosition
    View
    AI (Parallel)

*/