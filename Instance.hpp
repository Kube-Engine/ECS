/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Instance
 */

#include <tuple>
#include <bitset>

#include "Component.hpp"

#ifndef KUBE_ECS_COMPONENT_COUNT_LIMIT
# define KUBE_ECS_COMPONENT_COUNT_LIMIT 64
#endif

namespace kF::ECS
{
    class Instance;
    struct EntityDescriptor;

    constexpr ComponentIndex ComponentCountLimit = KUBE_ECS_COMPONENT_COUNT_LIMIT;
}

struct kF::ECS::EntityDescriptor
{
    std::bitset<ComponentCountLimit> components;

    [[nodiscard]] bool operator==(const EntityDescriptor &other) const noexcept{ return components == other.components; }
    [[nodiscard]] bool operator!=(const EntityDescriptor &other) const noexcept{ return components != other.components; }
};

class kF::ECS::Instance
{
public:
    [[nodiscard]] static Instance *Get(void) noexcept { return GetRef(); }

    [[nodiscard]] static Instance *&GetRef(void) noexcept {
        static Instance *instance = nullptr;
        return instance;
    }

    ~Instance(void);

    template<typename Type>
    [[nodiscard]] ComponentType registerComponent(void) noexcept;

    template<typename Type>
    [[nodiscard]] Component<Type> &getComponent(void) noexcept_ndebug;

    void releaseComponent(const ComponentIndex index);

    template<typename ...Args>
    [[nodiscard]] Entity registerEntity(void) noexcept { std::array<ComponentIndex, sizeof...(Args)> array; return registerEntity<Args...>(array); }

    template<typename ...Args>
    [[nodiscard]] Entity registerEntity(std::array<ComponentIndex, sizeof...(Args)> &array) noexcept;

    template<bool ReleaseComponents = true>
    void releaseEntity(const EntityIndex index);

    template<typename ...Args>
    [[nodiscard]] EntityType getEntityType(void) noexcept;

private:
    std::vector<std::aligned_storage_t<sizeof(Component<int>)>> _components;
    std::vector<EntityDescriptor> _descriptors;
    std::vector<EntityType> _entities;
    std::vector<EntityIndex> _freeEntities;

    template<std::size_t Index, typename Tuple>
    inline void getEntityTypeImpl(EntityDescriptor &descriptor) noexcept;

    template<typename ...Args>
    [[nodiscard]] Entity registerEntityImpl(void) noexcept;

    template<std::size_t Index, typename Tuple>
    inline void addEntityComponents(const EntityIndex index, std::array<ComponentIndex, std::tuple_size_v<Tuple>> &array);
};

#include "Instance.ipp"