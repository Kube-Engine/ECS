/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Helper to get unique opaque instance
 */

#pragma once

#include "ComponentTable.hpp"

namespace kF::ECS
{
    template<typename EntityType>
    struct KF_ALIGN_QUARTER_CACHELINE OpaqueComponentTable
    {
        using RemoveFunc = void(*)(void *instance, const EntityType entity);
        using DestroyFunc = void(*)(void *instance);

        RemoveFunc removeFunc;
        DestroyFunc destroyFunc;
    };

    static_assert(sizeof(OpaqueComponentTable<Entity>) == Core::CacheLineQuarterSize, "OpaqueComponentTable must take quarter of a cacheline");
    static_assert(alignof(OpaqueComponentTable<Entity>) == Core::CacheLineQuarterSize, "OpaqueComponentTable must be aligned to quarter of a cacheline");

    template<typename Component, typename EntityType>
    struct UniqueOpaqueComponent
    {
        using Table = ComponentTable<Component, EntityType>;

        static inline const OpaqueComponentTable<EntityType> Instance {
            removeFunc: [](void *instance, const EntityType entity) {
                if (const auto table = reinterpret_cast<Table *>(instance); table->exists(entity))
                    table->remove(entity);
            },
            destroyFunc: [](void *instance) {
                reinterpret_cast<Table *>(instance)->~Table();
            }
        };
    };

    template<typename Component, typename EntityType>
    [[nodiscard]] static const OpaqueComponentTable<EntityType> *GetOpaqueComponentTable(void) noexcept
    {
        using FlatComponent = std::remove_cvref_t<Component>;

        return &UniqueOpaqueComponent<FlatComponent, EntityType>::Instance;
    }
}