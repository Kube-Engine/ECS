/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: ECS View
 */

#pragma once

#include <tuple>

#include "ComponentTable.hpp"

namespace kF::ECS
{
    template<EntityRequirements EntityType, typename ...Components>
        requires (sizeof...(Components) > 0)
    class View;

    // template<typename Component, typename ...Components>
    // concept ViewTraversableRequirements = std::bool_constant<sizeof...(Components) == 0 && (!std::is_same_v<Component, Components> && ...)>;
}

template<kF::ECS::EntityRequirements EntityType, typename ...Components>
    requires (sizeof...(Components) > 0)
class kF::ECS::View
{
public:
    /** @brief Construct the view */
    View(ComponentTable<Components, EntityType> &...components) noexcept
        : _tables(std::make_tuple<ComponentTable<Components, EntityType> *...>(&components...)) {}

    /** @brief Copy constructor */
    View(const View &other) noexcept = default;

    /** @brief Copy assignment */
    View &operator=(const View &other) noexcept = default;

    /** @brief Traverse the view and call 'func' for each match and return true if functor has been called at least once */
    template<typename Functor>
    bool traverse(Functor &&func) const;

    /** @brief Traverse the view and call 'func' for each match and return true if functor has been called at least once. Enforce the iteration order in case of Component */
    template<typename Component, typename Functor>
//        requires (!std::is_same_v<Component, Components> && ...)
    bool traverse(Functor &&func) const;

    /** @brief Collect all entities which match and return entites in the Container */
    template<typename Container>
    void collect(Container &) const;

    /** @brief Collect all entities which match and return entites in the Container. Enforce the iteration order in case of Component */
    template<typename Component, typename Container>
    void collect(Container &) const;

private:
    /** @brief Get entities of the component with the minimum amount of entities which match */
    [[nodiscard]] const Core::Vector<EntityType, EntityType> *findMinimumEntities() const noexcept;

    /** @brief Get a specific component from a referenced table */
    template<typename Component>
    [[nodiscard]] Component &getComponentOf(EntityType entity) const noexcept;

    std::tuple<ComponentTable<Components, EntityType> *...> _tables;
};

#include "View.ipp"
