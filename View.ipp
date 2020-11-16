/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: ECS View
 */

template<kF::ECS::EntityRequirements EntityType, typename ...Components>
template<typename Functor>
inline bool kF::ECS::View<EntityType, Components ...>::traverse(Functor &&func) const
{
    const auto entities = findMinimumEntities();
    bool success = false;

    ((&(std::get<ComponentTable<Components, EntityType> *>(_tables)->getEntities()) == entities ? success = traverse<Components>(std::move(func)) : bool()), ...);
    return success;
}

template<kF::ECS::EntityRequirements EntityType, typename ...Components>
template<typename Component, typename Functor>
//    requires (!std::is_same_v<Component, Components> && ...)
inline bool kF::ECS::View<EntityType, Components ...>::traverse(Functor &&func) const
{
    bool success = false;

    for (const auto entity : std::get<ComponentTable<Component, EntityType> *>(_tables)->getEntities()) {
        if (((std::is_same_v<Component, Components> || std::get<ComponentTable<Components, EntityType> *>(_tables)->exists(entity)) && ...)) {
            func(getComponentOf<Components>(entity)...);
            success = true;
        }
    }
    return success;
}

template<kF::ECS::EntityRequirements EntityType, typename ...Components>
template<typename Container>
inline void kF::ECS::View<EntityType, Components ...>::collect(Container &container) const
{
    const auto entities = findMinimumEntities();

    ((&(std::get<ComponentTable<Components, EntityType> *>(_tables)->getEntities()) == entities ? collect<Components>(container) : void()), ...);
}

template<kF::ECS::EntityRequirements EntityType, typename ...Components>
template<typename Component, typename Container>
inline void kF::ECS::View<EntityType, Components ...>::collect(Container &container) const
{
    for (const auto entity : std::get<ComponentTable<Component, EntityType> *>(_tables)->getEntities()) {
        if (((std::is_same_v<Component, Components> || std::get<ComponentTable<Components, EntityType> *>(_tables)->exists(entity)) && ...)) {
            container.push(entity);
        }
    }
}

template<kF::ECS::EntityRequirements EntityType, typename ...Components>
inline const kF::Core::Vector<EntityType, EntityType> *kF::ECS::View<EntityType, Components ...>::findMinimumEntities() const noexcept
{
    return std::min(
        {
            &(std::get<ComponentTable<Components, EntityType> *>(_tables)->getEntities())...
        },
        [](const auto first, const auto second) {
            return first->size() < second->size();
        }
    );
}

template<kF::ECS::EntityRequirements EntityType, typename ...Components>
template<typename Component>
inline Component &kF::ECS::View<EntityType, Components ...>::getComponentOf(EntityType entity) const noexcept
{
    return std::get<ComponentTable<Component, EntityType> *>(_tables)->get(entity);
}
