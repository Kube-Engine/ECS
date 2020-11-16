/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of View
 */

#include <gtest/gtest.h>

#include <Kube/ECS/Registry.hpp>
#include <Kube/Flow/Scheduler.hpp>
#include <iostream>

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
class SystemPhysics : public ECS::ASystem<EntityType>
{
public:
    using ECS::ASystem<EntityType>::Dependencies;
    using ECS::ASystem<EntityType>::graph;

    SystemPhysics(void) noexcept : ECS::ASystem<EntityType>(typeid(SystemPhysics)) {};
    virtual ~SystemPhysics(void) override = default;

    virtual void setup(ECS::Registry<ECS::Entity> &registry) override
    {
        graph().emplace([&registry, this] {
            Core::Vector<EntityType> entities;
            registry.view<Position, Velocity, Rigidbody>().collect(entities);

            auto &componentTables = registry.componentTables();
            auto &tablePosition = componentTables.getTable<Position>();
            auto &tableVelocity = componentTables.getTable<Velocity>();
            auto &tableRigidbody = componentTables.getTable<Rigidbody>();
            auto end = entities.cend();
            auto beforeEnd = end - 1;

            for (auto it = entities.cbegin(); it != end && it != beforeEnd; it++) {
                auto &position1 = tablePosition.get(*it);
                auto &velocity1 = tableVelocity.get(*it);
                auto &rigidbody1 = tableRigidbody.get(*it);
                bool hasCollide = false;

                position1.x += velocity1.x;
                position1.y += velocity1.y;
                for (auto testIt = it + 1; testIt != end; testIt++) {
                    auto &position2 = tablePosition.get(*testIt);
                    auto &rigidbody2 = tableRigidbody.get(*testIt);

                    if (collide(position1, rigidbody1, position2, rigidbody2)) {
                        hasCollide = true;
                        rigidbody1.collideDispatcher.dispatch(rigidbody2);
                        rigidbody2.collideDispatcher.dispatch(rigidbody1);
                    }
                }
                if (hasCollide) {
                    position1.x -= velocity1.x;
                    position1.y -= velocity1.y;
                }
            }
        } );
    }

    virtual ECS::ASystem<EntityType>::Dependencies dependencies(void) override
    {
        return {};
    }

private:
    inline bool collide(const Position &position1, const Rigidbody &rigidbody1, const Position &position2, const Rigidbody &rigidbody2)
    {
        return position1.x < position2.x + rigidbody2.width && position1.y < position2.y + rigidbody2.height
            && position1.x + rigidbody1.width > position2.x && position1.y + rigidbody1.height > position2.y;
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
        graph().emplace([&registry, this] {
            Core::Vector<EntityType> entities;
            registry.view<Position, CharacterData>().collect(entities);

            auto &componentTables = registry.componentTables();
            auto &tablePosition = componentTables.getTable<Position>();
            auto &tableCharacterData = componentTables.getTable<CharacterData>();
            auto begin = entities.cbegin();
            auto end = entities.cend();

            for (auto it = begin; it != end; it++) {
                auto &position1 = tablePosition.get(*it);
                auto &characterData1 = tableCharacterData.get(*it);

                for (auto testIt = begin; testIt != end; testIt++) {
                    if (it != testIt) {
                        auto &position2 = tablePosition.get(*testIt);
                        auto &characterData2 = tableCharacterData.get(*testIt);

                        if (characterData1.range >= (position2.x - position1.x) + (position2.y - position1.y)) {
                            characterData2.life -= characterData1.damage;
                        }
                    }
                }
            }
        });
    }

    virtual ECS::ASystem<EntityType>::Dependencies dependencies(void) override
    {
        return { typeid(SystemPhysics<EntityType>) };
    }
};

TEST(ECS, Basics)
{
    ECS::Registry<ECS::Entity> registry;

    registry.registerComponent<Position>();
    registry.registerComponent<Velocity>();
    registry.registerComponent<Rigidbody>();
    registry.registerComponent<CharacterData>();

    auto player = registry.add();
    auto &playerPosition = registry.attach<Position>(player, 0, 0);
    registry.attach<Velocity>(player, 1, 0);
    auto &playerRigidbody = registry.attach<Rigidbody>(player, 1, 2);
    auto &playerData = registry.attach<CharacterData>(player, 100, 15, 1);

    auto enemy = registry.add();
    auto &enemyPosition = registry.attach<Position>(enemy, 10, 0);
    registry.attach<Velocity>(enemy, 0, 0);
    auto &enemyRigidbody = registry.attach<Rigidbody>(enemy, 1, 1);
    auto &enemyData = registry.attach<CharacterData>(enemy, 75, 10, 2);

    auto &systemGraph = registry.systemGraph();
    systemGraph.add<SystemPhysics<ECS::Entity>>();
    systemGraph.add<SystemAI<ECS::Entity>>();
    registry.buildSystemGraph();

    Flow::Scheduler scheduler;
    for (int i = 0; i < 20; i += 1) {
        std::cout << "Round " << i << std::endl;

        scheduler.schedule(registry);
        systemGraph.graph().wait();

        std::cout << "Player" << std::endl;
        std::cout << "Position(x[" << playerPosition.x << "] y[" << playerPosition.y << "])" << std::endl;
        std::cout << "Rigidboy(width[" << playerRigidbody.width << "] height[" << playerRigidbody.height << "])" << std::endl;
        std::cout << "Data(life[" << playerData.life << "]" << std::endl;

        std::cout << std::endl;

        std::cout << "Enemy" << std::endl;
        std::cout << "Position(x[" << enemyPosition.x << "] y[" << enemyPosition.y << "])" << std::endl;
        std::cout << "Rigidboy(width[" << enemyRigidbody.width << "] height[" << enemyRigidbody.height << "])" << std::endl;
        std::cout << "Data(life[" << enemyData.life << "]" << std::endl;

        std::cout << std::endl << std::endl;
    }
}
