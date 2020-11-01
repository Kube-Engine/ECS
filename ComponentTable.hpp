/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

#pragma once

#include <Core/FlatVector.hpp>

#include "Base.hpp"
#include "SparseIndexSet.hpp"

namespace kF::ECS
{
    template<typename Component, typename EntityType>
    class ComponentTable;
}

/** @brief Store all instances of a component type in a registry */
template<typename Component, typename EntityType = kF::ECS::Entity>
class kF::ECS::ComponentTable
{
public:
    using Iterator = Core::FlatVector<Component>::Iterator;
    using ConstIterator = Core::FlatVector<Component>::ConstIterator;

    /** @brief Add a component linked to a given entity */
    template<typename ...Args>
    void add(const EntityType entity, Args &&...args);

    /** @brief Remove a component linked to a given entity */
    void remove(const EntityType entity);

    /** @brief Begin / end iterators */
    [[nodiscard]] Iterator begin(void) noexcept { return _components.begin(); }
    [[nodiscard]] ConstIterator begin(void) const noexcept { return _components.begin(); }
    [[nodiscard]] ConstIterator cbegin(void) const noexcept { return _components.cbegin(); }
    [[nodiscard]] Iterator end(void) noexcept { return _components.end(); }
    [[nodiscard]] ConstIterator end(void) const noexcept { return _component.end(); }
    [[nodiscard]] ConstIterator cend(void) const noexcept { return _components.cend(); }

private:
    SparseIndexSet<std::uint32_t, PageSize, UINT32_MAX> _indexes {};
    Core::FlatVector<Component> _components {};
};

#include "ComponentTable.ipp"
