/**
 * @ Author: ...
 * @ Description: ...
 */

#pragma once

namespace kF::ECS
{
    template <typename EntityType>
    class ComponentTables;
}

/** @brief ... */
template <typename EntityType>
class ComponentTables
{
public:
    /** @brief Construct the ComponentTables */
    ComponentTables(void) noexcept = default;

    /** @brief Destroy the ComponentTables */
    ~ComponentTables(void) = default;
};