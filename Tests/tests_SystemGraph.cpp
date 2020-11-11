l/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Unit tests of Registry
 */

#include <iostream>
#include <typeindex>
#include <gtest/gtest.h>

#include <Kube/ECS/Registry.hpp>
#include <Kube/ECS/SystemGraph.hpp>
#include <Kube/ECS/ASystem.hpp>

using namespace kF;

using TypeID = std::type_index;
using Dependencies = std::vector<TypeID>;

template <typename EntityType>
class SystemA: public ECS::ASystem<EntityType>
{
public:
    SystemA() noexcept : ECS::ASystem<EntityType>(typeid(SystemA)) {};

    void setup(ECS::Registry<ECS::Entity> &registry) { std::cout << "SETUP SYSTEM A" << std::endl; };

    Dependencies dependencies(void) {
        std::vector<TypeID> test;
        return test;
    };
};

template <typename EntityType>
class SystemB: public ECS::ASystem<EntityType>
{
public:
    SystemB() noexcept : ECS::ASystem<EntityType>(typeid(SystemB)) {};

    void setup(ECS::Registry<ECS::Entity> &registry) { std::cout << "SETUP SYSTEM B" << std::endl; };

    Dependencies dependencies(void) {
        std::vector<TypeID> test;
        test.push_back(typeid(SystemA<EntityType>));
        return test;
    }
};

TEST(SystemGraph, Build)
{
    ECS::Registry<ECS::Entity> registry;
    ECS::SystemGraph<ECS::Entity> systemGraph;

    systemGraph.add<SystemA<ECS::Entity>>();
    // systemGraph.add<SystemB<ECS::Entity>>();
    // systemGraph.build(registry);
}
