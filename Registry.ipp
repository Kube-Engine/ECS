/**
 * @ Author: Léos Julien
 * @ Description: Used to create Entities, add Components to them, retrieve Systems etc...
 */

template<typename EntityType>
inline EntityType kF::ECS::Registry<EntityType>::add(void) noexcept_ndebug
{
    EntityType newEntity = EntityType();
    _entities.push_back(newEntity);

    return newEntity;
}

template<typename EntityType>
template<typename... Components>
inline EntityType kF::ECS::Registry<EntityType>::add(Components &&... components)
    noexcept(nothrow_ndebug && (nothrow_forward_constructible(Components)...))
{
    EntityType newEntity = EntityType();
    if (_lastDestroyed == undefined) {
    _entities.push_back(newEntity);
    attach(newEntity, components);

    return newEntity;
}

// EntityType &getNullEntity(index)
// if (entity[index] != NullEntity)
//      return getNullEntity(entity[index])
// return entity[index]

// 10 entities

// remove entity #3
// if (lastDestroyed == undefined)
//      entity[3] = NullEntity
// lastDestroyed = 3

// remove entity #6
// entity[6] = lastDestroyed = 3
// lastDestroyed = 6

// add entity
// getNullEntity(lastDestroyed) = entity[3] = newEntity




// 10 entities

// remove entity #0
// lastDestroyed = 0

// add entity
// entities[lastDestroyed = 0] = newEntity
// lastDestroyed = 

// remove entity #1
// entities[lastDestroyed = 0] = NullEntity
// lastDestroyed = 1; 

template<typename EntityType>
inline void kF::ECS::Registry<EntityType>::remove(const EntityType entity) noexcept_ndebug
{
    const auto it = std::find(_entities.begin(), _entities.end(), entity);
    if (it == _systems.end()) {
        throw std::logical_error("ECS::Registry: Entity does not exists")
    }

    const std::uint64_t index = std::distance(_entities.begin(), it); // index type should be an int of sizeof(EntityType) bytes
    _entities[index] = _lastDestroyed == undefined ? kF::ECS::NullEntity : 
    if (_lastDestroyed == undefined) {
        _entities[index] = kF::ECS::NullEntity;
    } else {
        _entities[index] = _lastDestroyed;
    }
    _lastDestroyed = index;

    std::remove(_entities.begin(), _entities.end(), entity);
}

template<typename EntityType>
template<typename... Components>
inline void kF::ECS::Registry<EntityType>::remove(const EntityType entity) noexcept_ndebug
{

}

template<typename Component, typename... Args>
Component &kF::ECS::Registry<EntityType>::attach(const EntityType entity, Args &&... args)
    noexcept(nothrow_ndebug && nothrow_constructible(Component, Args...))
{
    if (_componentTables.tableExists<Component>() == false) {
        _componentTables.add<Component>();
    }

    auto componentTable = _componentTables.getTable<Component>();
    componentTable.add(entity, args);

    return componentTable.get(entity);
}

template<typename... Components>
inline void kF::ECS::Registry<EntityType>::attach(const EntityType entity, Components &&... components)
    noexcept(nothrow_ndebug && (nothrow_constructible(Components)...))
{
    for (auto component : components) {
        std::type_info componentType = typeid(component);

        if (_componentTables.tableExists<componentType>() == false) {
            _componentTables.add<componentType>();
        }

        _componentTables.getTable<componentType>().add(entity);
    }
}

template<typename EntityType>
template<typename Component>
inline void kF::ECS::Registry<EntityType>::detach(const EntityType entity)
    noexcept(nothrow_ndebug && nothrow_destructible(Component))
{

}

template<typename EntityType>
template<typename... Components>
inline void kF::ECS::Registry<EntityType>::detach(const EntityType entity)
    noexcept(nothrow_ndebug && (nothrow_destructible(Components)...))
{

}

template<typename EntityType>
inline void kF::ECS::Registry<EntityType>::clear(void)
{

}

template<typename EntityType>
template<typename... Components>
inline kF::ECS::View<EntityType, Component...> kF::ECS::Registry<EntityType>::view(void) const noexcept_ndebug
{

}

template<typename EntityType>
inline EntityType &getNullEntity(const std::uint64_t index) const noexcept
{
    if (_entities[index] != kF::ECS::NullEntity) {
        return getNullEntity(_entities[index]);
    }
    return _entities[index];
}