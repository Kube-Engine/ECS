/**
 * @ Author: Léos Julien
 * @ Description: An opaque system
 */
#pragma once

#include <iostream>
#include <vector>
#include <typeindex>

#include <Kube/Flow/Graph.hpp>

#include "Base.hpp"

namespace kF::ECS
{
    template <typename EntityType>
    class ASystem;

    template <typename EntityType>
    class Registry;
}

/** @brief An opaque system */
template <typename EntityType>
class kF::ECS::ASystem
{
public:
    using TypeID = std::type_index;
    using Dependencies = std::vector<TypeID>;

    /** @brief Construct a new system using a TypeID */
    ASystem(const TypeID typeID) noexcept : _typeID(typeID) {};

    /** @brief Destruct the system */
    virtual ~ASystem<EntityType>(void) = default;


    /** @brief Setup the system */
    virtual void setup(Registry<EntityType> &registry) = 0;

    /** @brief Get dependecies of the system */
    [[nodiscard]] virtual Dependencies dependencies(void) = 0;


    /** @brief Get system's TypeID */
    [[nodiscard]] TypeID typeID(void) const noexcept { return _typeID; };


    /** @brief Get system's internal Graph */
    [[nodiscard]] kF::Flow::Graph &graph(void) noexcept { return _graph; };

    /** @brief Get system's internal Graph */
    [[nodiscard]] const kF::Flow::Graph &graph(void) const noexcept { return _graph; }


    /** @brief Get system's internal Task */
    [[nodiscard]] kF::Flow::Task &task(void) noexcept { return _task; };

    /** @brief Get system's internal Task */
    [[nodiscard]] const kF::Flow::Task &task(void) const noexcept { return _task; };

private:
    const TypeID _typeID;
    kF::Flow::Graph _graph {};
    kF::Flow::Task _task {};
};