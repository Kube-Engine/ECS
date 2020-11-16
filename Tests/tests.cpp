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

struct Rigidbody
{
    using CollideDispatcher = Core::TrivialDispatcher<void (ECS::Entity entity)>;

    int width;
    int height;
    int dx;
    int dy;
    CollideDispatcher collideDispatcher;
};

struct CharacterData
{
    using TargetChangedDispatcher = Core::TrivialDispatcher<void (CharacterData &)>;

    enum Type {
        CAC,
        DIST,
    };

    Type type;
    int life;
    int damage;
    int range;
    TargetChangedDispatcher targetChanged;
};

struct Arrow
{
    int damage;
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
            registry.view<Position, Rigidbody>().collect(entities);

            auto &componentTables = registry.componentTables();
            auto &tablePosition = componentTables.getTable<Position>();
            auto &tableRigidbody = componentTables.getTable<Rigidbody>();
            auto end = entities.cend();
            auto beforeEnd = end - 1;

            for (auto it = entities.cbegin(); it != end && it != beforeEnd; it++) {
                auto &position1 = tablePosition.get(*it);
                auto &rigidbody1 = tableRigidbody.get(*it);
                bool hasCollide = false;

                position1.x += rigidbody1.dx;
                position1.y += rigidbody1.dy;
                for (auto testIt = it + 1; testIt != end; testIt++) {
                    auto &position2 = tablePosition.get(*testIt);
                    auto &rigidbody2 = tableRigidbody.get(*testIt);

                    if (collide(position1, rigidbody1, position2, rigidbody2)) {
                        hasCollide = true;
                        rigidbody1.collideDispatcher.dispatch(*testIt);
                        rigidbody2.collideDispatcher.dispatch(*it);
                    }
                }
                if (hasCollide) {
                    position1.x -= rigidbody1.dx;
                    position1.y -= rigidbody1.dy;
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

                        if (characterData1.range >= std::abs(position2.x - position1.x) + std::abs(position2.y - position1.y)) {
                            characterData1.targetChanged.dispatch(characterData2);
                            if (characterData1.type == CharacterData::CAC)
                                characterData2.life -= characterData1.damage;
                            else {
                                auto arrow = registry.add();
                                registry.attach<Position>(arrow, position1.x, position1.y);
                                auto &rigidbody = registry.attach<Rigidbody>(arrow, 1, 1, -1, 0);
                                auto &arrowComponent = registry.attach<Arrow>(arrow, characterData1.damage);

                                rigidbody.collideDispatcher.add([&tableCharacterData, arrow, &arrowComponent, &registry](EntityType entity) {
                                    auto &other = tableCharacterData.get(entity);

                                    other.life -= arrowComponent.damage;
                                    registry.remove(arrow);
                                });
                            }
                            break;
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
    registry.registerComponent<Rigidbody>();
    registry.registerComponent<CharacterData>();

    auto player = registry.add();
    auto &playerPosition = registry.attach<Position>(player, 0, 0);
    auto &playerRigidbody = registry.attach<Rigidbody>(player, 1, 1, 1, 0);
    auto &playerData = registry.attach<CharacterData>(player, CharacterData::CAC, 100, 15, 1);

    auto enemy1 = registry.add();
    auto &enemy1Position = registry.attach<Position>(enemy1, 10, 0);
    auto &enemy1Rigidbody = registry.attach<Rigidbody>(enemy1, 1, 1, 0, 0);
    auto &enemy1Data = registry.attach<CharacterData>(enemy1, CharacterData::CAC, 75, 5, 1);

//    auto enemy2 = registry.add();
//    auto &enemy2Position = registry.attach<Position>(enemy2, 9, 10);
//    auto &enemy2Rigidbody = registry.attach<Rigidbody>(enemy2, 1, 1, 0, -1);
//    std::cout << "Rigidboy(width[" << playerRigidbody.width << "] height[" << playerRigidbody.height << "])" << std::endl;
//    auto &enemy2Data = registry.attach<CharacterData>(enemy2, CharacterData::DIST, 50, 10, 3);

//    std::cout << "Rigidboy(width[" << playerRigidbody.width << "] height[" << playerRigidbody.height << "])" << std::endl;

    auto &systemGraph = registry.systemGraph();
    systemGraph.add<SystemPhysics<ECS::Entity>>();
    systemGraph.add<SystemAI<ECS::Entity>>();
    registry.buildSystemGraph();

//    std::cout << "Rigidboy(width[" << playerRigidbody.width << "] height[" << playerRigidbody.height << "])" << std::endl;

    Flow::Scheduler scheduler;
    for (int i = -1; i < 20; i += 1) {
        std::cout << "Round " << i << std::endl;

        if (i >= 0) {
            scheduler.schedule(registry);
            systemGraph.graph().wait();
        }

        std::cout << std::endl << "Player" << std::endl;
        std::cout << "Position(x[" << playerPosition.x << "] y[" << playerPosition.y << "])" << std::endl;
        std::cout << "Rigidboy(width[" << playerRigidbody.width << "] height[" << playerRigidbody.height << "])" << std::endl;
        std::cout << "Data(life[" << playerData.life << "]" << std::endl;

        std::cout << std::endl << "Enemy1" << std::endl;
        std::cout << "Position(x[" << enemy1Position.x << "] y[" << enemy1Position.y << "])" << std::endl;
        std::cout << "Rigidboy(width[" << enemy1Rigidbody.width << "] height[" << enemy1Rigidbody.height << "])" << std::endl;
        std::cout << "Data(life[" << enemy1Data.life << "]" << std::endl;

//        std::cout << std::endl << "Enemy2" << std::endl;
//        std::cout << "Position(x[" << enemy2Position.x << "] y[" << enemy2Position.y << "])" << std::endl;
//        std::cout << "Rigidboy(width[" << enemy2Rigidbody.width << "] height[" << enemy2Rigidbody.height << "])" << std::endl;
//        std::cout << "Data(life[" << enemy2Data.life << "]" << std::endl;

        std::cout << std::endl << std::endl;
    }
}
