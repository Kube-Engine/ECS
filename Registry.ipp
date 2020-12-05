/**
 * @ Author: Léos Julien
 * @ Description: Used to create Entities, add Components to them, retrieve Systems etc...
 */

#include <tuple>

template<kF::ECS::EntityRequirements EntityType>
inline EntityType kF::ECS::Registry<EntityType>::add(void) noexcept
{
    // Check if there is a free entity
    if (_lastDestroyed != NullEntity<EntityType>) [[likely]] {
        const auto freeEntity = _entities.begin() + _lastDestroyed;
        _lastDestroyed = *freeEntity; // Store the next freed entity into 'lastDestroyed'
        *freeEntity = std::distance(_entities.begin(), freeEntity);
        return *freeEntity;
    // If not, add another entity to the list
    } else [[unlikely]]
        return _entities.push(_entities.size());
}

template<kF::ECS::EntityRequirements EntityType>
template<typename... Components>
inline EntityType kF::ECS::Registry<EntityType>::add(Components &&... components)
    noexcept(nothrow_ndebug && (... && nothrow_forward_constructible(decltype(components))))
{
    const auto newEntity = add();

    attach<Components...>(newEntity, std::forward<Components>(components)...);
    return newEntity;
}

template<kF::ECS::EntityRequirements EntityType>
inline void kF::ECS::Registry<EntityType>::remove(const EntityType entity) noexcept_ndebug
{
    removeEntityFromRegistry(entity);
    _componentTables.removeEntity(entity);
}

template<kF::ECS::EntityRequirements EntityType>
template<typename... Components>
inline void kF::ECS::Registry<EntityType>::remove(const EntityType entity) noexcept_ndebug
{
    removeEntityFromRegistry(entity);
    detach<Components...>(entity);
}

template<kF::ECS::EntityRequirements EntityType>
template<typename Component, typename... Args>
inline Component &kF::ECS::Registry<EntityType>::attach(const EntityType entity, Args &&... args)
    noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...))
{
    kFAssert(_componentTables.template tableExists<Component>(),
        throw std::logic_error("ECS::Registry::attach: ComponentTable does not exists"));
    return _componentTables.template getTable<Component>().add(entity, std::forward<Args>(args)...);
}

template<kF::ECS::EntityRequirements EntityType>
template<typename... Components> requires (sizeof...(Components) > 1)
inline void kF::ECS::Registry<EntityType>::attach(const EntityType entity, Components &&... components)
    noexcept(nothrow_ndebug && (... && nothrow_forward_constructible(decltype(components))))
{
    (... , attach<Components>(entity, std::forward<Components>(components)));
}

template<kF::ECS::EntityRequirements EntityType>
template<typename Component>
inline void kF::ECS::Registry<EntityType>::detach(const EntityType entity)
    noexcept(nothrow_ndebug &&nothrow_destructible(Component))
{
    kFAssert(_componentTables.template tableExists<Component>(),
             throw std::logic_error("ECS::Registry::detach: ComponentTable does not exists"));
    _componentTables.template getTable<Component>().remove(entity);
}

template<kF::ECS::EntityRequirements EntityType>
template<typename... Components> requires (sizeof...(Components) > 1)
inline void kF::ECS::Registry<EntityType>::detach(const EntityType entity)
    noexcept(nothrow_ndebug && (... && nothrow_destructible(Components)))
{
    kFAssert((... && _componentTables.template tableExists<Components>()),
             throw std::logic_error("ECS::Registry::detach: ComponentTable does not exists"));
    (... , detach<Components>(entity));
}

template<kF::ECS::EntityRequirements EntityType>
inline void kF::ECS::Registry<EntityType>::clear(void)
{
    _componentTables.clear();
    _entities.clear();
    _lastDestroyed = NullEntity<EntityType>;
    _systemGraph.clear();
}

template<kF::ECS::EntityRequirements EntityType>
template<typename... Components>
inline kF::ECS::View<EntityType, Components...> kF::ECS::Registry<EntityType>::view(void) noexcept_ndebug
{
    kFAssert((... && _componentTables.template tableExists<Components>()),
        throw std::logic_error("ECS::Registry::detach: ComponentTable does not exists"));

    return View<EntityType, Components...>(
        _componentTables.template getTable<Components>()...
    );
}

template<kF::ECS::EntityRequirements EntityType>
inline void kF::ECS::Registry<EntityType>::removeEntityFromRegistry(const EntityType entity) noexcept_ndebug
{
    const auto lastDestroyed = _lastDestroyed;

    _lastDestroyed = entity;
    _entities.at(entity) = lastDestroyed;
}