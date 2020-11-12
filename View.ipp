/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: ECS View
 */

template<typename EntityType, typename ...Components>
template<typename Functor>
bool kF::ECS::View<EntityType, Components ...>::traverse(Functor &&func) const
{
    if constexpr (sizeof...(Components) == 0) [[unlikely]]
        return false;
    else [[likely]] {
        const auto entities = std::min({&(std::get<ComponentTable<Components, EntityType> *>(_tables)->getEntities())...}, [](const auto first, const auto second) {
            return first->size() < second->size();
        });
        bool success = false;

        ((&(std::get<ComponentTable<Components, EntityType> *>(_tables)->getEntities()) == entities ? success = traverse<Components>(std::move(func)) : bool()), ...);
        return success;
    }
}

template<typename EntityType, typename ...Components>
template<typename Component, typename Functor>
bool kF::ECS::View<EntityType, Components ...>::traverse(Functor &&func) const
{
    if constexpr (sizeof...(Components) == 0 || (!std::is_same_v<Component, Components> && ...)) [[unlikely]]
        return false;
    else [[likely]] {
        bool success = false;

        for (const auto entity : std::get<ComponentTable<Component, EntityType> *>(_tables)->getEntities()) {
            if (((std::is_same_v<Component, Components> || std::get<ComponentTable<Components, EntityType> *>(_tables)->exists(entity)) && ...)) {
                func(getComponentOf<Components>(entity)...);
                success = true;
            }
        }
        return success;
    }
}

template<typename EntityType, typename ...Components>
template<typename Component>
const Component &kF::ECS::View<EntityType, Components ...>::getComponentOf(EntityType entity) const noexcept
{
    return std::get<ComponentTable<Component, EntityType> *>(_tables)->get(entity);
}
