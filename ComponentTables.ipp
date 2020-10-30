/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTables
 */

template<typename EntityType>
template<typename Component>
void kF::ECS::ComponentTables<EntityType>::add<Component>(void)
{
}

template<typename EntityType>
template<typename Component>
bool kF::ECS::ComponentTables<EntityType>::tableExists<Component>(void) const
{
}

template<typename EntityType>
bool kF::ECS::ComponentTables<EntityType>::tableExists(const Meta::Type type) const
{
}

template<typename EntityType>
template<typename Component>
[[nodiscard]] kF::ECS::ComponentTable<Component> &kF::ECS::ComponentTables<EntityType>::getTable<Component>(void) const
{
}

template<typename EntityType>
template<typename Component>
[[nodiscard]] const kF::ECS::ComponentTable<Component> &kF::ECS::ComponentTables<EntityType>::getTable<Component>(void) const
{
}

template<typename EntityType>
void kF::ECS::ComponentTables<EntityType>::clear(void)
{
}
