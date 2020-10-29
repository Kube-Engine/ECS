/**
 * @ Author: Léos Julien
 * @ Description: Used to create Entities, add Components to them, retrieve Systems etc...
 */

#pragma once

#include <vector>

#include "SystemGraph.hpp"
#include "ComponentTables.hpp"

namespace kF::ECS
{
    template <typename EntityType>
    class Registry;
}

/** @brief Used to create Entities, add Components to them, retrieve Systems etc... */
template <typename EntityType>
class alignas(128) Registry
{
public:
    /** @brief Construct the Registry */
    Registry(void) noexcept = default;

    /** @brief Destroy the Registry */
    ~Registry(void) = default;


    /** @brief Construct an empty entity */
    [[nodiscard]] EntityType add(void) noexcept;

    /** @brief Construct an entity with several components binded */
    template<typename... Components>
    EntityType add(Components &&... components);


    /** @brief Slow opaque entity erasure */
    void remove(const EntityType entity);

    /** @brief Fast explicit entity erasure */
    template<typename... Components>
    void remove(const EntityType entity);


    /** @brief Add a single component to an entity  */
    template<typename Component>
    [[nodiscard]] Component attach(const EntityType entity);

    /** @brief Add a single component to an entity with a set of predefined arguments */
    template<typename Component, typename... Args>
    void attach(const EntityType entity, Args &&... args);

    /** @brief Add a set of components to an entity */
    template<typename... Component>
    void attach(const EntityType entity, Components &&... components);


    /** @brief Remove a single component from an entity */
    template<typename Component>
    void detach(const EntityType entity);

    /** @brief Remove a set of components from an entity */
    template<typename... Component>
    void detach(const EntityType entity);


    /** @brief Clear the whole registry (components, systems, entities) */
    void clear(void);


    /** @brief Create a view used to traverse entities matching a set of components */
    template<typename... Component>
    [[nodiscard]] View<EntityType, Component...> view(void) const;


    /** @brief Retreive the component table list */
    [[nodiscard]] ComponentTables &componentTables(void) { return _componentTables; };

    /** @brief Retreive the component table list */
    [[nodiscard]] const ComponentTables &componentTables(void) const { return _componentTables; };


    /** @brief Retreive the graph of systems */
    [[nodiscard]] SystemGraph &systemGraph(void) { return _systemGraph; };

    /** @brief Retreive the graph of systems */
    [[nodiscard]] const SystemGraph &systemGraph(void) const { return _systemGraph; };


    // Miss operators

private:
    ComponentTables<EntityType> _componentTables;
    SystemGraph _systemGraph;

    std::vector<EntityType> _entities;
    EntityType _lastDestroyed;
};