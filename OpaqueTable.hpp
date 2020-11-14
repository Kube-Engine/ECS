/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Helper to get unique opaque instance
 */

#pragma once

#include "ComponentTable.hpp"

namespace kF::ECS
{
    template<EntityRequirements EntityType>
    struct alignas_quarter_cacheline OpaqueComponentTable
    {
        using RemoveFunc = void(*)(void *instance, const EntityType entity);
        using DestroyFunc = void(*)(void *instance);

        RemoveFunc removeFunc;
        DestroyFunc destroyFunc;
    };

    static_assert_fit_quarter_cacheline(OpaqueComponentTable<ShortEntity>);
    static_assert_fit_quarter_cacheline(OpaqueComponentTable<Entity>);
    static_assert_fit_quarter_cacheline(OpaqueComponentTable<LongEntity>);

    template<typename Component, EntityRequirements EntityType>
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

    template<typename Component, EntityRequirements EntityType>
    [[nodiscard]] static const OpaqueComponentTable<EntityType> *GetOpaqueComponentTable(void) noexcept
    {
        using FlatComponent = std::remove_cvref_t<Component>;

        return &UniqueOpaqueComponent<FlatComponent, EntityType>::Instance;
    }
}