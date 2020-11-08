/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

#include <stdexcept>

#include <Kube/Core/Assert.hpp>

template <typename Component, typename EntityType>
template <typename... Args>
inline Component &kF::ECS::ComponentTable<Component, EntityType>::add(const EntityType entity, Args &&... args) noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...))
{
    _indexes.add(entity);
    return _components.push(std::forward<Args>(args)...);
}

template <typename Component, typename EntityType>
inline void kF::ECS::ComponentTable<Component, EntityType>::remove(const EntityType entity) noexcept(nothrow_ndebug && nothrow_destructible(Component))
{
    kFAssert(_indexes.exists(entity),
        throw std::logic_error("ECS::ComponentTable::remove: Entity doesn't exists"));

    // Move the last component to index given by sparse set
    const auto lastIndex = _indexes.entityCount() - 1;
    const auto toRemoveIndex = _indexes.remove(entity);
    _components.at(toRemoveIndex) = std::move(_components.at(lastIndex));
    _components.pop();
}

template <typename Component, typename EntityType>
inline const Component &kF::ECS::ComponentTable<Component, EntityType>::get(const EntityType entity) const noexcept_ndebug
{
    kFAssert(_indexes.exists(entity),
        throw std::logic_error("ECS::ComponentTable::get: Entity doesn't exists"));

    return _components.at(_indexes.at(entity));
}

template <typename Component, typename EntityType>
inline void kF::ECS::ComponentTable<Component, EntityType>::clear(void)
{
    _components.clear();
    _indexes.clear();
}