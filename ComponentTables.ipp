/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTables
 */

template<typename EntityType>
template<typename Component>
inline void kF::ECS::ComponentTables<EntityType>::add(void)
{
}

template<typename EntityType>
inline bool kF::ECS::ComponentTables<EntityType>::tableExists(const Meta::Type type) const noexcept
{
}

template<typename EntityType>
template<typename Component>
inline const kF::ECS::ComponentTable<Component> &kF::ECS::ComponentTables::getTable<Component>(void) const noexcept_ndebug
{
}

template<typename EntityType>
inline void kF::ECS::ComponentTables<EntityType>::clear(void)
{
}
