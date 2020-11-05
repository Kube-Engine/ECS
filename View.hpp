/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: ECS View
 */

#pragma once

#include <tuple>

#include "ComponentTable.hpp"

namespace kF::ECS
{
    template<typename EntityType, typename ...Components>
    class View;
}

template<typename EntityType, typename ...Components>
class kF::ECS::View
{
public:
    /** @brief Construct the view */
    View(ComponentTable<Components> &...components) noexcept
        : _tables(std::make_tuple<ComponentTable<Components> *...>(&components...)) {}

    /** @brief Copy constructor */
    View(const View &other) noexcept = default;

    /** @brief Copy assignment */
    View &operator=(const View &other) noexcept = default;

    /** @brief Traverse the view and call 'func' for each match and return true if functor has been called at least once */
    template<typename Functor>
    bool traverse(Functor &&func);

private:
    std::tuple<ComponentTable<Components> *...> _tables;
};