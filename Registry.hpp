/**
 * @ Author: Léos Julien
 * @ Description: Used to create Entities, add Components to them, retrieve Systems etc...
 */

#pragma once

#include "View.hpp"
#include "SystemGraph.hpp"
#include "ComponentTables.hpp"

namespace kF::ECS
{
    template<EntityRequirements EntityType>
    class Registry;
}

/** @brief Used to create Entities, add Components to them, retrieve Systems etc... */
template<kF::ECS::EntityRequirements EntityType>
class alignas_double_cacheline kF::ECS::Registry
{
public:
    /** @brief Construct the Registry */
    Registry(void) noexcept = default;

    /** @brief Destroy the Registry */
    ~Registry(void) = default;


    /** @brief Register a component type into the registry */
    template<typename Component>
    void registerComponent(void) noexcept_ndebug { _componentTables.template add<Component>(); }


    /** @brief Construct an empty entity */
    [[nodiscard("You may not discard an entity without components")]]
    EntityType add(void) noexcept;

    /** @brief Construct an entity with one component binded */
    template<typename Component, typename ...Args>
    EntityType add(Args &&...args)
        noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...))
        { return add<Component>(Component(std::forward<Args>(args)...)); }

    /** @brief Construct an entity with several components binded */
    template<typename... Components>
    EntityType add(Components &&... components)
        noexcept(nothrow_ndebug && (... && nothrow_forward_constructible(decltype(components))));

    /** @brief Slow opaque entity erasure */
    void remove(const EntityType entity) noexcept_ndebug;

    /** @brief Fast explicit entity erasure */
    template<typename... Components>
    void remove(const EntityType entity) noexcept_ndebug;


    /** @brief Add a single component to an entity with a set of predefined arguments */
    template<typename Component, typename... Args>
    Component &attach(const EntityType entity, Args &&... args)
        noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...));

    /** @brief Add a set of components to an entity */
    template<typename... Components> requires (sizeof...(Components) > 1)
    void attach(const EntityType entity, Components &&... components)
        noexcept(nothrow_ndebug && (... && nothrow_forward_constructible(decltype(components))));


    /** @brief Remove a single component from an entity */
    template<typename Component>
    void detach(const EntityType entity)
        noexcept(nothrow_ndebug && nothrow_destructible(Component));

    /** @brief Remove a set of components from an entity */
    template<typename... Components> requires (sizeof...(Components) > 1)
    void detach(const EntityType entity)
        noexcept(nothrow_ndebug && (... && nothrow_destructible(Components)));

    /** @brief Clear the whole registry (components, systems, entities) */
    void clear(void);


    /** @brief Create a view used to traverse entities matching a set of components */
    template<typename... Components>
    [[nodiscard]] View<EntityType, Components...> view(void) noexcept_ndebug;


    /** @brief Retreive the component table list */
    [[nodiscard]] ComponentTables<EntityType> &componentTables(void) noexcept { return _componentTables; };

    /** @brief Retreive the component table list */
    [[nodiscard]] const ComponentTables<EntityType> &componentTables(void) const noexcept { return _componentTables; };


    /** @brief Retreive the graph of systems */
    [[nodiscard]] SystemGraph<EntityType> &systemGraph(void) noexcept { return _systemGraph; };

    /** @brief Retreive the graph of systems */
    [[nodiscard]] const SystemGraph<EntityType> &systemGraph(void) const noexcept { return _systemGraph; };

    /** @brief Build system graph */
    void buildSystemGraph(void) { _systemGraph.build(*this); }


    /** @brief Implicit conversion to Flow::Graph, used for Scheduler */
    [[nodiscard]] operator Flow::Graph &(void) noexcept { return _systemGraph.graph(); }
    [[nodiscard]] operator const Flow::Graph &(void) const noexcept { return _systemGraph.graph(); }

private:
    ComponentTables<EntityType> _componentTables {};
    Core::Vector<EntityType, EntityType> _entities {};
    EntityType _lastDestroyed { NullEntity<EntityType> };
    alignas_cacheline SystemGraph<EntityType> _systemGraph {};

    /** @brief Only remove an entity from _entities vector */
    void removeEntityFromRegistry(const EntityType entity) noexcept_ndebug;
};

static_assert_fit_double_cacheline(kF::ECS::Registry<kF::ECS::ShortEntity>);
static_assert_fit_double_cacheline(kF::ECS::Registry<kF::ECS::Entity>);
static_assert_fit_double_cacheline(kF::ECS::Registry<kF::ECS::LongEntity>);

#include "SystemGraph.hpp"
#include "Registry.ipp"