/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTables
 */

#pragma once

#include <array>

#include <Kube/Core/Vector.hpp>
#include <Kube/Core/FlatVector.hpp>

#include "OpaqueTable.hpp"

namespace kF::ECS
{
    template<typename EntityType>
    class ComponentTables;
}

/** @brief Store all component tables of a registry */
template<typename EntityType>
class KF_ALIGN_HALF_CACHELINE kF::ECS::ComponentTables
{
public:
    static constexpr std::size_t ComponentTableSize = sizeof(ComponentTable<std::nullptr_t, EntityType>);

    /** @brief Helper types */
    using OpaqueTable = const OpaqueComponentTable<EntityType> *;
    using RemoveFunc = OpaqueComponentTable<EntityType>::RemoveFunc;

    /** @brief Construct the ComponentTables */
    ComponentTables(void) noexcept = default;

    /** @brief Destroy the ComponentTables */
    ~ComponentTables(void) { clear(); }

    /** @brief Add a ComponentTable to internal list */
    template<typename Component>
    void add(void) noexcept_ndebug;

    /** @brief Check if a ComponentTable is registered internally using explicit type */
    template<typename Component>
    [[nodiscard]] bool tableExists(void) const noexcept { return tableExists(GetOpaqueComponentTable<Component, EntityType>()); }

    /** @brief Check if a ComponentTable is registered internally using an opaque type */
    [[nodiscard]] bool tableExists(const OpaqueTable opaqueTable) const noexcept;

    /** @brief Get a ComponentTable casted to the right component */
    template<typename Component>
    [[nodiscard]] ComponentTable<Component, EntityType> &getTable(void) noexcept_ndebug
        { return const_cast<ComponentTable<Component, EntityType> &>(const_cast<const ComponentTables<EntityType> &>(*this).getTable<Component>()); }

    /** @brief Get a ComponentTable casted to the right component */
    template<typename Component>
    [[nodiscard]] const ComponentTable<Component, EntityType> &getTable(void) const noexcept_ndebug;

    /** @brief Get the number of ComponentTable stored internally */
    [[nodiscard]] std::size_t size(void) const noexcept { return _opaqueTables.size(); }

    /** @brief Removes an entity from every opaque table */
    void removeEntity(const EntityType entity);

    /** @brief Clear every table and remove them */
    void clear(void);

private:
    Core::TinyVector<OpaqueTable> _opaqueTables {};
    Core::FlatVector<RemoveFunc, std::uint32_t> _removeFuncs {};
    Core::FlatVector<std::array<std::byte, ComponentTableSize>> _tables {};
};

static_assert(sizeof(kF::ECS::ComponentTables<kF::ECS::ShortEntity>) == kF::Core::CacheLineSize / 2, "ComponentTables must take half a cacheline");
static_assert(alignof(kF::ECS::ComponentTables<kF::ECS::ShortEntity>) == kF::Core::CacheLineSize / 2, "ComponentTables must be aligned to half a cacheline");
static_assert(sizeof(kF::ECS::ComponentTables<kF::ECS::Entity>) == kF::Core::CacheLineSize / 2, "ComponentTables must take half a cacheline");
static_assert(alignof(kF::ECS::ComponentTables<kF::ECS::Entity>) == kF::Core::CacheLineSize / 2, "ComponentTables must be aligned to half a cacheline");
static_assert(sizeof(kF::ECS::ComponentTables<kF::ECS::LongEntity>) == kF::Core::CacheLineSize / 2, "ComponentTables must take half a cacheline");
static_assert(alignof(kF::ECS::ComponentTables<kF::ECS::LongEntity>) == kF::Core::CacheLineSize / 2, "ComponentTables must be aligned to half a cacheline");

#include "ComponentTables.ipp"
