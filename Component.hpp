/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Component
 */

#pragma once

#include <utility>

#include "Entity.hpp"

namespace kF::ECS
{
    class IComponent;

    template<typename Data>
    class Component;

    using ComponentIndex = std::uint32_t;
    using ComponentType = std::uint32_t;

    constexpr ComponentIndex NullComponentIndex = std::numeric_limits<ComponentIndex>::max();
    constexpr ComponentType NullComponentType = std::numeric_limits<ComponentType>::max();
}

class kF::ECS::IComponent
{
public:
    virtual ~IComponent(void) = default;

    virtual void release(const ComponentIndex index) = 0;

    virtual void releaseEntity(const EntityIndex index) = 0;
};

template<typename Data>
class kF::ECS::Component final : public IComponent
{
public:
    static ComponentType Type(void) noexcept { return TypeRef(); }
    static ComponentType &TypeRef(void) noexcept {
        static ComponentType type { NullComponentType };
        return type;
    }

    ~Component(void) final = default;

    template<typename ...Args>
    [[nodiscard]] ComponentIndex add(const EntityIndex index, Args &&...args);

    inline void release(const ComponentIndex index) final;

    [[nodiscard]] ComponentIndex getEntityIndex(const EntityIndex index) const noexcept;

    [[nodiscard]] Data &get(const ComponentIndex index) noexcept { return _values[index]; }
    [[nodiscard]] const Data &get(const ComponentIndex index) const noexcept { return _values[index]; }

    void releaseEntity(const EntityIndex index) final;

    [[nodiscard]] std::size_t size(void) const noexcept { return _indexes.size() - _freeSlots.size(); }

private:
    std::vector<EntityIndex> _indexes;
    std::vector<Data> _values;
    std::vector<ComponentIndex> _freeSlots;
};

static_assert(sizeof(kF::ECS::Component<int>) == sizeof(kF::ECS::Component<std::vector<int>>), "A Component size must not vary upon instantiation");
static_assert(sizeof(kF::ECS::Component<int>) == sizeof(kF::ECS::Component<char[420]>), "A Component size must not vary upon instantiation");

#include "Component.ipp"