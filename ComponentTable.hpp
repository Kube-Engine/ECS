/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

#pragma once

#include <Kube/Core/TrivialDispatcher.hpp>

#include "SparseEntitySet.hpp"

namespace kF::ECS
{
    template<typename Component, EntityRequirements EntityType>
    class ComponentTable;
}

/** @brief Store all instances of a component type in a registry */
template<typename Component, kF::ECS::EntityRequirements EntityType>
class alignas_double_cacheline kF::ECS::ComponentTable
{
public:
    /** @brief Size of a page (in elements, not in bytes) */
    static constexpr EntityType PageSize = 16384u / sizeof(EntityType);

    /** @brief Vector of components */
    using Components = Core::Vector<Component, EntityType>;

    /** @brief Iterator over components */
    using Iterator = Components::Iterator;

    /** @brief Readonly iterator over components */
    using ConstIterator = Components::ConstIterator;

    /** @brief Dispatcher when an entity is added */
    using AddDispatcher = Core::TrivialDispatcher<void (EntityType)>;

    /** @brief Dispatcher when an entity is removed */
    using RemoveDispatcher = Core::TrivialDispatcher<void (EntityType)>;

    /** @brief Check if an entity exists in the table */
    [[nodiscard]] bool exists(const EntityType entity) const noexcept { return _indexes.exists(entity); }

    /** @brief Add a component linked to a given entity */
    template<typename... Args>
    Component &add(const EntityType entity, Args &&... args)
        noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...));

    /** @brief Remove a component linked to a given entity */
    void remove(const EntityType entity)
        noexcept(nothrow_ndebug && nothrow_destructible(Component));

    /** @brief Get all entities */
    [[nodiscard]] const Core::Vector<EntityType, EntityType> &getEntities(void) const noexcept { return _indexes.flatset(); }

    /** @brief Get the component of a given entity */
    [[nodiscard]] Component &get(const EntityType entity) noexcept_ndebug
        { return const_cast<Component &>(const_cast<const ComponentTable &>(*this).get(entity)); }
    [[nodiscard]] const Component &get(const EntityType entity) const noexcept_ndebug;

    /** @brief Clear */
    void clear(void);

    /** @brief Get the size of the table */
    [[nodiscard]] std::size_t size(void) noexcept { return _components.size(); }

    /** @brief Begin / end iterators */
    [[nodiscard]] Iterator begin(void) noexcept { return _components.begin(); }
    [[nodiscard]] ConstIterator begin(void) const noexcept { return _components.begin(); }
    [[nodiscard]] ConstIterator cbegin(void) const noexcept { return _components.cbegin(); }
    [[nodiscard]] Iterator end(void) noexcept { return _components.end(); }
    [[nodiscard]] ConstIterator end(void) const noexcept { return _components.end(); }
    [[nodiscard]] ConstIterator cend(void) const noexcept { return _components.cend(); }

    /** @brief Get add dispacher */
    [[nodiscard]] AddDispatcher &getAddDispatcher(void) noexcept { return _addDispatcher; }

    /** @brief Get remove dispacher */
    [[nodiscard]] RemoveDispatcher &getRemoveDispatcher(void) noexcept { return _removeDispatcher; }

private:
    SparseEntitySet<EntityType, PageSize> _indexes {};
    Components _components {};
    AddDispatcher _addDispatcher {};
    RemoveDispatcher _removeDispatcher {};
};

static_assert_fit_double_cacheline(TEMPLATE_TYPE(kF::ECS::ComponentTable, std::nullptr_t, kF::ECS::ShortEntity));
static_assert_fit_double_cacheline(TEMPLATE_TYPE(kF::ECS::ComponentTable, std::nullptr_t, kF::ECS::Entity));
static_assert_fit_double_cacheline(TEMPLATE_TYPE(kF::ECS::ComponentTable, std::nullptr_t, kF::ECS::LongEntity));

#include "ComponentTable.ipp"
