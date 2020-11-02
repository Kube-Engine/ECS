/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

#include <stdexcept>

template<typename Component, typename EntityType>
template<typename ...Args>
inline void kF::ECS::ComponentTable<Component, EntityType>::add(const EntityType entity, Args &&...args) noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...))
{
    _indexes.add(entity);
    _components.push(std::forward<Args>(args)...);
}

template<typename Component, typename EntityType>
inline void kF::ECS::ComponentTable<Component, EntityType>::remove(const EntityType entity) noexcept_ndebug
{
    kFAssert(_indexes.exists(entity),
        throw std::range_error("ECS::ComponentTable::remove: Entity doesn't exists"));

    // Move the last component to index given by sparse set
    const auto lastIndex = _indexes.entityCount() - 1;
    const auto toRemoveIndex = _indexes.remove(entity);
    _components.atUnsafe(toRemoveIndex) = std::move(_components.atUnsafe(lastIndex)); // atUnsafe because we don't want checks
    _components.pop(); // Remove the last one
}

template<typename Component, typename EntityType>
inline void kF::ECS::ComponentTable<Component, EntityType>::clear(void)
{
    _components.clear();
    _indexes.clear();
}

template<typename Component, typename EntityType>
inline const Component &kF::ECS::ComponentTable<Component, EntityType>::get(const EntityType entity) const noexcept_ndebug
{
    kFAssert(_indexes.exists(entity),
        throw std::range_error("ECS::ComponentTable::get: Entity doesn't exists"));

    const auto componentIndex = _indexes.at(entity);

    return _components.atUnsafe(componentIndex); // atUnsafe because we don't want checks
}