/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

#pragma once

#include <Kube/Core/FlatVector.hpp>

#include "Base.hpp"
#include "SparseEntitySet.hpp"

namespace kF::ECS
{
    template<typename Component, typename EntityType>
    class ComponentTable;
}

/** @brief Store all instances of a component type in a registry */
template<typename Component, typename EntityType = kF::ECS::Entity>
class alignas(32) kF::ECS::ComponentTable
{
public:
    /** @brief Iterator over components */
    using Iterator = Core::FlatVector<Component>::Iterator;

    /** @brief Readonly iterator over components */
    using ConstIterator = Core::FlatVector<Component>::ConstIterator;

    /** @brief Size of a page (in elements, not in bytes) */
    constexpr EntityType PageSize = 16384u / sizeof(EntityType);


    /** @brief Add a component linked to a given entity */
    template<typename ...Args>
    void add(const EntityType entity, Args &&...args) noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...));

    /** @brief Remove a component linked to a given entity */
    void remove(const EntityType entity) noexcept_ndebug;

    /** @brief Clear */
    void clear(void);

    /** @brief Get the component of a given entity */
    [[nodiscard]] Component &get(const EntityType entity) noexcept_ndebug { return const_cast<Component &>(const_cast<const ComponentTable &>(*this).get<Component>()); }
    [[nodiscard]] const Component &get(const EntityType entity) const noexcept_ndebug;


    /** @brief Begin / end iterators */
    [[nodiscard]] Iterator begin(void) noexcept { return _components.begin(); }
    [[nodiscard]] ConstIterator begin(void) const noexcept { return _components.begin(); }
    [[nodiscard]] ConstIterator cbegin(void) const noexcept { return _components.cbegin(); }
    [[nodiscard]] Iterator end(void) noexcept { return _components.end(); }
    [[nodiscard]] ConstIterator end(void) const noexcept { return _component.end(); }
    [[nodiscard]] ConstIterator cend(void) const noexcept { return _components.cend(); }

private:
    SparseEntitySet<EntityType, PageSize> _indexes {};
    Core::FlatVector<Component> _components {};
};

#include "ComponentTable.ipp"
