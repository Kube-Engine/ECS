/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: ECS Base
 */

#pragma once

#include <numeric>

#include <Kube/Core/Utils.hpp>

namespace kF::ECS
{
    /** @brief A tiny entity that can handle 65536 entities */
    using ShortEntity = std::uint16_t;

    /** @brief A standard entity that can handle 4294967296 entities */
    using Entity = std::uint32_t;

    /** @brief A large entity that can handle an indecent amount of pow(1.8, 19) entities */
    using LongEntity = std::uint64_t;

    /** @brief Entity concept requirements */
    template<typename EntityType>
    concept EntityRequirements = std::is_same_v<EntityType, ShortEntity> ||
            std::is_same_v<EntityType, Entity> || std::is_same_v<EntityType, LongEntity>;

    /** @brief Null entityt */
    template<EntityRequirements EntityType>
    constexpr auto NullEntity = std::numeric_limits<EntityType>::max();
}