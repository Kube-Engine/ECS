/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

template<typename Component, typename EntityType>
template<typename ...Args>
void kF::ECS::ComponentTable<Component, EntityType>::add(const EntityType entity, Args &&...args)
{
}

template<typename Component, typename EntityType>
void kF::ECS::ComponentTable<Component, EntityType>::remove(const EntityType entity)
{
    const std::size_t entityIndex = entity.index;
    const auto componentIndex = _indexes.remove(entityIndex);

    // Destroy the component
    // Use the ENTT technique to swap the last with destroyed one
    // Don't forget to change index of the last value
}
