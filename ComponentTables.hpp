/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTables
 */

#pragma once

#include <Kube/Meta/Type.hpp>

#include "ComponentTable.hpp"

namespace kF::ECS
{
    template<typename EntityType>
    class ComponentTables;
}

/** @brief Store all component tables of a registry */
template<typename EntityType = kF::ECS::Entity>
class alignas(16) kF::ECS::ComponentTables
{
public:
    /** @brief Construct the ComponentTables */
    ComponentTables(void) noexcept = default;

    /** @brief Destroy the ComponentTables */
    ~ComponentTables(void) = default;

    /** @brief Add a ComponentTable to internal list */
    template<typename Component>
    void add(void) noexcept_ndebug;

    /** @brief Check if a ComponentTable is registered internally using explicit type */
    template<typename Component>
    [[nodiscard]] bool tableExists(void) const noexcept { return tableExists(Meta::Factory<Component>::Resolve()); }

    /** @brief Check if a ComponentTable is registered internally using an opaque type */
    [[nodiscard]] bool tableExists(const Meta::Type type) const noexcept;

    /** @brief Get a ComponentTable casted to the right component */
    template<typename Component>
    [[nodiscard]] ComponentTable<Component> &getTable(void) const noexcept_ndebug
        { return const_cast<ComponentTable<Component> &>(const_cast<const ComponentTables &>(*this).getTable<Component>()); }

    /** @brief Get a ComponentTable casted to the right component */
    template<typename Component>
    [[nodiscard]] const ComponentTable<Component> &getTable(void) const noexcept_ndebug;

    /** @brief Get the number of ComponentTable stored internally */
    [[nodiscard]] std::size_t size(void) const noexcept { return _size; }

    /** @brief Clear every table and remove them */
    void clear(void);

private:
    std::unique_ptr<std::byte[]> _data {};
    std::size_t _size { 0ul };

    [[nodiscard]] Meta::Type *getTypeData(void) noexcept { return reinterpret_cast<Meta::Type>(_data.get()); }
    [[nodiscard]] const Meta::Type *getTypeData(void) const noexcept { return reinterpret_cast<Meta::Type>(_data.get()); }
};

#include "ComponentTables.ipp"
