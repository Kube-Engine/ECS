/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Entity Holder
 */

#pragma once

#include "ComponentHolder.hpp"

namespace kF::ECS
{
    template<typename ...Args>
    class EntityHolder;
}

template<typename ...Args>
class kF::ECS::EntityHolder
{
    template<std::size_t Index, typename Type>
    static constexpr std::size_t GetTypeIndex(void) noexcept;

    template<typename Type>
    static constexpr std::size_t TypeIndex = GetTypeIndex<0, Type>();
public:
    using ArgsTuple = std::tuple<Args...>;

    EntityHolder(void) noexcept;

    EntityHolder(EntityHolder &&other) noexcept { swap(other); }

    ~EntityHolder(void);

    EntityHolder &operator=(EntityHolder &&other) noexcept { swap(other); return *this; }

    void swap(EntityHolder &other) noexcept { std::swap(_entity, other._entity); _components.swap(other._components); }

    template<typename Type, std::enable_if_t<TypeIndex<Type> != sizeof...(Args)>* = nullptr>
    Type &get(void) noexcept { return Instance::Get()->getComponent<Type>().get(_components[TypeIndex<Type>]); }

    template<typename Type, std::enable_if_t<TypeIndex<Type> != sizeof...(Args)>* = nullptr>
    const Type &get(void) const noexcept { return Instance::Get()->getComponent<Type>().get(_components[TypeIndex<Type>()]); }

private:
    Entity _entity {};
    std::array<ComponentIndex, sizeof...(Args)> _components {};

    template<std::size_t Index>
    inline void releaseComponents(void);
};

#include "EntityHolder.ipp"