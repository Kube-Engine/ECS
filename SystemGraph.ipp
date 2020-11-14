/**
 * @ Author: Léos Julien
 * @ Description: A Flow Graph of System(s)
 */

#include <stdexcept>
#include <typeindex>
#include <memory>

template<kF::ECS::EntityRequirements EntityType>
template<typename System, typename... Args> requires std::derived_from<System, kF::ECS::ASystem<EntityType>> && std::constructible_from<System, Args...>
inline System &kF::ECS::SystemGraph<EntityType>::add(Args &&... args)
    noexcept(nothrow_ndebug && nothrow_constructible(System, Args...))
{
    kFAssert(!exists<System>(),
        throw std::logic_error("ECS::SystemGraph: System already exists"));

    auto systemPtr = std::make_unique<System>(std::forward<Args>(args)...);
    auto &system = *systemPtr;
    _systems.push(std::move(systemPtr));
    _graph.emplace(system.graph());
    return system;
}

template<kF::ECS::EntityRequirements EntityType>
template<typename System> requires std::derived_from<System, kF::ECS::ASystem<EntityType>>
inline bool kF::ECS::SystemGraph<EntityType>::exists(void) const noexcept
{
    return std::any_of(_systems.begin(), _systems.end(), [systemTypeID = std::type_index(typeid(System))](const SystemPtr<EntityType> &systemPtr) {
        return systemPtr->typeID() == systemTypeID;
    });
}

template<kF::ECS::EntityRequirements EntityType>
template<typename System> requires std::derived_from<System, kF::ECS::ASystem<EntityType>>
inline const System &kF::ECS::SystemGraph<EntityType>::get(void) const noexcept_ndebug
{
    const auto it = std::find_if(_systems.begin(), _systems.end(), [systemTypeID = std::type_index(typeid(System))](const SystemPtr<EntityType> &systemPtr) {
        return systemPtr->typeID() == systemTypeID;
    });

    kFAssert(it != _systems.end(),
        throw std::logic_error("ECS::SystemGraph: System does not exists"));
    return *dynamic_cast<System*>((*it).get());
}

template<kF::ECS::EntityRequirements EntityType>
inline void kF::ECS::SystemGraph<EntityType>::build(Registry<EntityType> &registry)
{
    using SystemPair = std::pair<ASystem<EntityType> *, typename ASystem<EntityType>::Dependencies>;

    if (_systems.size() == 0ul)
        throw std::logic_error("ECS::SystemGraph::build: No system in graph");

    // Clear graph and prepare order set
    Core::Vector<SystemPair> systemsOrder;
    _graph.clear();
    systemsOrder.reserve(_systems.size());

    // Collect data over each systems
    for (auto &system : _systems) {
        system->setup(registry);
        system->task() = _graph.emplace(system->graph());
        systemsOrder.push(system.get(), system->dependencies());
    }

    // Determine the sequential order of systems
    for (auto it = systemsOrder.begin(); it != systemsOrder.end();) {
        bool hasSwap = false;
        for (const auto &dependencyID : it->second) {
            const auto dependencyExist = [dependencyID](const SystemPair &pair){ return pair.first->typeID() == dependencyID; };
            // Swap `dependencyID` and `it` if `dependencyID` is not before `it`
            if (!std::any_of(systemsOrder.begin(), it, dependencyExist)) {
                auto dependencyIt = std::find_if(systemsOrder.begin(), systemsOrder.end(), dependencyExist);
                if (dependencyIt == systemsOrder.end())
                    throw std::logic_error("ECS::SystemGraph::build: Dependency do not exist");
                std::swap(it, dependencyIt);
                hasSwap = true;
                break;
            }
        }
        if (!hasSwap)
            ++it;
    }

    // Build the sequential graph
    auto current = systemsOrder.begin();
    auto next = current + 1;
    const auto end = systemsOrder.end();
    while (next != end) {
        current->first->task().precede(next->first->task());
        ++current;
        ++next;
    }
}

template<kF::ECS::EntityRequirements EntityType>
inline void kF::ECS::SystemGraph<EntityType>::clear(void) noexcept
{
    _systems.clear();
    _graph.clear();
}