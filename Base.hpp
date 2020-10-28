/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: ECS Base
 */

#pragma once

#include <Kube/Core/Core.hpp>

namespace kF::ECS
{
    struct Entity
    {
        std::uint8_t    version { 0u };
        std::uint32_t   index : 24 { 0u };
    };

    struct LongEntity
    {
        std::uint32_t version { 0u };
        std::uint32_t index { 0u };
    };
}