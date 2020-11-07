/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: ECS View
 */

#include "View.hpp"

template<typename EntityType, typename ...Components>
template<typename Functor>
bool kF::ECS::View<EntityType, Components ...>::traverse(Functor &&func) const
{
    const auto table = std::min(_tables, [](const auto *first, const auto *second) {
        return first.size() < second.size();
    });

    return ((std::get<ComponentTable<Components, EntityType> *>(_tables) == table ? traverse<Components>(std::move(func)) : void()), ...);
}

template<typename EntityType, typename ...Components>
template<typename Functor, typename Component>
bool kF::ECS::View<EntityType, Components ...>::traverse(Functor &&func) const
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

template<typename EntityType, typename ...Components>
template<typename Component>
const Component &kF::ECS::View<EntityType, Components ...>::getComponentOf(EntityType entity) const noexcept
{
    return std::get<ComponentTable<Component, EntityType> *>(_tables)->get(entity);
}
