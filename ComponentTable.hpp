/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

#pragma once

#include <Core/FlatVector.hpp>

#include "SparseSet.hpp"

struct Entity
{
};

namespace kF::ECS
{
    template<typename Component, typename EntityType>
    class ComponentTable;
}

template<typename Component, typename EntityType = Entity>
class kF::ECS::ComponentTable
{
public:
    template<typename ...Args>
    void add(const EntityType entity, Args &&...args);

    void remove(const EntityType entity);

    /** @brief Begin / end components */
    [[nodiscard]] Component *begin(void) const noexcept { return _components.begin(); }
    [[nodiscard]] const Component *begin(void) const noexcept { return _components.begin(); }
    [[nodiscard]] const Component *cbegin(void) const noexcept { return _components.cbegin(); }
    [[nodiscard]] Component *end(void) const noexcept { return _components.end(); }
    [[nodiscard]] const Component *end(void) const noexcept { return _component.end(); }
    [[nodiscard]] const Component *cend(void) const noexcept { return _components.cend(); }

private:
    SparseSet<uint32_t> _indexes;
    Core::FlatVector<Component> _components;
};

#include "ComponentTable.ipp"
