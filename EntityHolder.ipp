/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: EntityHolder
 */

#include "EntityHolder.hpp"

template<typename ...Args>
kF::ECS::EntityHolder<Args...>::EntityHolder(void) noexcept
{
    _entity = Instance::Get()->registerEntity<Args...>(_components);
}

template<typename ...Args>
kF::ECS::EntityHolder<Args...>::~EntityHolder(void)
{
    releaseComponents<0>();
    if (_entity.index != NullComponentIndex)
        Instance::Get()->releaseEntity<false>(_entity.index);
}


template<typename ...Args>
template<std::size_t Index, typename Type>
constexpr std::size_t kF::ECS::EntityHolder<Args...>::GetTypeIndex(void) noexcept
{

    if constexpr (Index < sizeof...(Args)) {
        if constexpr (std::is_same_v<std::tuple_element_t<Index, ArgsTuple>, Type>)
            return Index;
        else
            return GetTypeIndex<Index + 1, Type>();
    } else
        return sizeof...(Args);
}

template<typename ...Args>
template<std::size_t Index>
void kF::ECS::EntityHolder<Args...>::releaseComponents(void)
{
    if constexpr (Index < sizeof...(Args)) {
        using CurrentType = std::tuple_element_t<Index, ArgsTuple>;

        Instance::Get()->getComponent<CurrentType>().release(_components[Index]);
        releaseComponents<Index + 1>();
    }
}