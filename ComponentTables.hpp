/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTables
 */

#pragma once

#include <Meta/Type.hpp>

#include "ComponentTable.hpp"

namespace kF::ECS
{
    template<typename EntityType>
    class ComponentTables;
}

template<typename EntityType = Entity>
class kF::ECS::ComponentTables
{
public:
    /** @brief Construct the ComponentTables */
    ComponentTables(void) noexcept = default;

    /** @brief Destroy the ComponentTables */
    ~ComponentTables(void) = default;

    /** @brief Add a ComponentTable to internal list */
    template<typename Component>
    void add<Component>(void);

    /** @brief Check if a ComponentTable is registered internally using explicit type */
    template<typename Component>
    bool tableExists<Component>(void) const;

    /** @brief Check if a ComponentTable is registered internally using an opaque type */
    bool tableExists(const Meta::Type type) const;

    /** @brief Get a ComponentTable casted to the right component */
    template<typename Component>
    [[nodiscard]] ComponentTable<Component> &getTable<Component>(void) const;

    /** @brief Get a ComponentTable casted to the right component */
    template<typename Component>
    [[nodiscard]] const ComponentTable<Component> &getTable<Component>(void) const;

    /** @brief Get the number of ComponentTable stored internally */
    [[nodiscard]] std::size_t size(void) const noexcept { return _size; }

    /** @brief Clear every table and remove them */
    void clear(void);

private:
    std::unique_ptr<std::byte[]> _data;
    std::size_t _size;
};

#include "ComponentTables.ipp"
