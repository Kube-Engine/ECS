/**
 * @ Author: Léos Julien
 * @ Description: Used to create Entities, add Components to them, retrieve Systems etc...
 */

template<typename EntityType>
EntityType kF::ECS::Registry<EntityType>::add(void) noexcept_ndebug
{

}

template<typename EntityType>
template<typename... Components>
EntityType kF::ECS::Registry<EntityType>::add(Components &&... components)
    noexcept(nothrow_ndebug && (nothrow_forward_constructible(Components)...))
{

}

template<typename EntityType>
void kF::ECS::Registry<EntityType>::remove(const EntityType entity)
{

}

template<typename... Components>
void kF::ECS::Registry<EntityType>::remove(const EntityType entity)
{

}

template<typename Component, typename... Args>
Component &kF::ECS::Registry<EntityType>::attach(const EntityType entity, Args &&... args)
    noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...))
{

}

template<typename... Components>
void kF::ECS::Registry<EntityType>::attach(const EntityType entity, Components &&... components)
    noexcept(nothrow_ndebug && (nothrow_constructible(Components)...))
{

}

template<typename EntityType>
template<typename Component>
void kF::ECS::Registry<EntityType>::detach(const EntityType entity)
    noexcept(nothrow_ndebug && nothrow_destructible(Component))
{

}

template<typename EntityType>
template<typename... Components>
void kF::ECS::Registry<EntityType>::detach(const EntityType entity)
    noexcept(nothrow_ndebug && (nothrow_destructible(Components)...))
{

}

template<typename EntityType>
void kF::ECS::Registry<EntityType>::clear(void)
{

}

template<typename EntityType>
template<typename... Components>
kF::ECS::View<EntityType, Component...> kF::ECS::Registry<EntityType>::view(void) const noexcept_ndebug
{

}