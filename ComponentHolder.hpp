/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Entity Holder
 */

#pragma once

#include "Instance.hpp"

namespace kF::ECS
{
    template<typename Type>
    class ComponentHolder;
}

template<typename Type>
class kF::ECS::ComponentHolder
{
public:
    ComponentHolder(void) noexcept = default;

    ComponentHolder(const ComponentIndex index) noexcept : _index(index) {}

    ComponentHolder(ComponentHolder &&other) noexcept { swap(other); }

    ~ComponentHolder(void) {
        if (_index != NullComponentIndex)
           Instance::Get()->getComponent<Type>().release(_index);
    }

    [[nodiscard]] ComponentHolder &operator=(ComponentHolder &&other) noexcept { swap(other); return *this; }

    void swap(ComponentHolder &other) noexcept { std::swap(_index, other._index); }

private:
    ComponentIndex _index { NullComponentIndex };
};