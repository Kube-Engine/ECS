/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Helper to get unique opaque instance
 */

#pragma once

#include "ComponentTable.hpp"

namespace kF::ECS
{
    template<typename EntityType>
    struct alignas(16) OpaqueComponentTable
    {
        using RemoveFunc = void(*)(void *instance, const EntityType entity);
        using DestroyFunc = void(*)(void *instance);

        RemoveFunc removeFunc;
        DestroyFunc destroyFunc;
    };

    static_assert(sizeof(OpaqueComponentTable<Entity>) == 16ul, "OpaqueComponentTable must take 16 bytes");
    static_assert(alignof(OpaqueComponentTable<Entity>) == 16ul, "OpaqueComponentTable must be aligned to 16 bytes");

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