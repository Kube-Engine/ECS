/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Entity
 */

#pragma once

#include <vector>

#include "Base.hpp"

namespace kF::ECS
{
    class Entity;

    using EntityIndex = std::uint32_t;
    using EntityType = std::uint32_t;

    constexpr EntityIndex NullEntityIndex = std::numeric_limits<EntityIndex>::max();
    constexpr EntityType NullEntityType = std::numeric_limits<EntityType>::max();
}

struct kF::ECS::Entity
{
    EntityIndex index { NullEntityIndex };
    EntityType type { NullEntityType };
};
