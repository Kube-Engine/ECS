/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTable
 */

template<typename Component, typename EntityType>
template<typename ...Args>
void kF::ECS::ComponentTable<Component, EntityType>::add(const EntityType entity, Args &&...args)
{
    const std::size_t entityIndex = entity.index;

    _indexes.add(entityIndex, entity);
    _components.push(std::forward<Args>(args)...);
}

template<typename Component, typename EntityType>
void kF::ECS::ComponentTable<Component, EntityType>::remove(const EntityType entity)
{
    const std::size_t entityIndex = entity.index;
    auto lastIndex = _components.size() - 1;
    auto lastEntity = std::move(_indexes.at(lastIndex));
    auto lastComponent = std::move(_components.back());

    // Destroy the component ??

    _indexes[entityIndex] = std::move(lastEntity);
    _components[entityIndex] = std::move(lastComponent);

//    const auto componentIndex = _indexes.remove(entityIndex); // Utils ? componentIndex == entity.index
    _indexes.remove(lastIndex); // But dont throw if entity isn't contained
    _components.pop();

}
