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
}
