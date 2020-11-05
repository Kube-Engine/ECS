/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

#include <stdexcept>

template<typename Component, typename EntityType>
template<typename ...Args>
inline Component &kF::ECS::ComponentTable<Component, EntityType>::add(const EntityType entity, Args &&...args) noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...))
{
    _indexes.add(entity);
    return _components.push(std::forward<Args>(args)...);
}

template<typename Component, typename EntityType>
inline void kF::ECS::ComponentTable<Component, EntityType>::remove(const EntityType entity) noexcept_ndebug
{
    kFAssert(_indexes.exists(entity),
        throw std::range_error("ECS::ComponentTable::remove: Entity doesn't exists"));

    // Move the last component to index given by sparse set
    const auto lastIndex = _indexes.entityCount() - 1;
    const auto toRemoveIndex = _indexes.remove(entity);
    _components.at(toRemoveIndex) = std::move(_components.at(lastIndex)); // at because we don't want checks
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

    return _components.at(componentIndex); // at because we don't want checks
}