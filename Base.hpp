/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: ECS Base
 */

#pragma once

#include <numeric>

#include <Kube/Core/Core.hpp>

namespace kF::ECS
{
    using ShortEntity = std::uint16_t;
    using Entity = std::uint32_t;
    using LongEntity = std::uint64_t;

    template<typename EntityType>
    constexpr auto NullEntity = std::numeric_limits<EntityType>::max();
}