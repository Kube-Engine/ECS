/**
 * @ Author: Léos Julien
 * @ Description: A Flow Graph of System(s)
 */

template<typename EntityType>
template <typename System, typename... Args>
    requires    std::derived_from<System, kF::ECS::ASystem<EntityType>> &&
                std::constructible_from<System, Args...>
inline System &kF::ECS::SystemGraph<EntityType>::add(Args &&... args)
    noexcept(nothrow_ndebug && nothrow_constructible(System, Args...))
{
    kFAssert(exists<System>(),
        throw std::logical_error("ECS::SystemGraph: System already exists")
    );

    auto newSystem = std::make_unique<System>(std::forward<Args>(args)...);
    _systems.emplace_back(newSystem);
    _graph.emplace(newSystem.graph());
}

template<typename EntityType>
template <typename System> requires std::derived_from<System, kF::ECS::ASystem<EntityType>>
inline bool kF::ECS::SystemGraph<EntityType>::exists(void) const noexcept
{
    const auto systemTypeID = typeid(System);
    const auto it = std::find_if(_systems.begin(), _systems.end(), [systemTypeID](SystemPtr<EntityType> &systemPtr) {
        return systemPtr->typeID() == systemTypeID;
    });

    return it != _systems.end();
}

template<typename EntityType>
template <typename System> requires std::derived_from<System, kF::ECS::ASystem<EntityType>>
inline const System &kF::ECS::SystemGraph<EntityType>::get(void) const
{
    const auto systemTypeID = typeid(System);
    const auto it = std::find_if(_systems.begin(), _systems.end(), [systemTypeID](SystemPtr<EntityType> &systemPtr) {
        return systemPtr->typeID() == systemTypeID;
    });

    if (it != _systems.end()) [[likely]]
        return *it;
    else [[unlikely]]
        throw std::logic_error("ECS::SystemGraph: System does not exists");
}

template<typename EntityType>
inline void kF::ECS::SystemGraph<EntityType>::clear(void)
{
    _systems.clear();
    _graph.clear();
}