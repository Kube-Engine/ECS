/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Component
 */

#include "Component.hpp"

template<typename Data>
template<typename ...Args>
kF::ECS::ComponentIndex kF::ECS::Component<Data>::add(const EntityIndex entityIndex, Args &&...args) //noexcept(kF::ECS::Component<Data>::NoexceptData)
{
    ComponentIndex componentIndex;

    if (!_freeSlots.empty()) {
        componentIndex = _freeSlots.back();
        _freeSlots.pop_back();
        _indexes[componentIndex] = entityIndex;
        _values[componentIndex] = Data(std::forward<Args>(args)...);
    } else {
        componentIndex = _indexes.size();
        _indexes.emplace_back(entityIndex);
        _values.emplace_back(std::forward<Args>(args)...);
    }
    return componentIndex;
}

template<typename Data>
void kF::ECS::Component<Data>::release(const ComponentIndex index)
{
    _indexes[index] = NullEntityIndex;
    _freeSlots.emplace_back(index);
}

template<typename Data>
kF::ECS::ComponentIndex kF::ECS::Component<Data>::getEntityIndex(const EntityIndex index) const noexcept
{
    for (ComponentIndex i = 0; const auto entity : _indexes) {
        if (entity != index) {
            ++i;
            continue;
        }
        return i;
    }
    return NullComponentIndex;
}

template<typename Data>
void kF::ECS::Component<Data>::releaseEntity(const EntityIndex index)
{
    for (ComponentIndex i = 0; auto entity : _indexes) {
        if (entity != index) {
            ++i;
            continue;
        }
        release(i);
        break;
    }
}
