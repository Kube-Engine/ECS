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
    using SystemPtr = ASystem<EntityType> *;

    if (_systems.size() == 0ul)
        throw std::logic_error("ECS::SystemGraph::build: No system in graph");

    // Prepare systems order set and clear graph 
    Core::Vector<SystemPtr> systemsOrder;
    systemsOrder.reserve(_systems.size());
    _graph.clear();

    // Collect data over each systems
    for (auto &system : _systems) {
        system->setup(registry);
        system->task() = _graph.emplace(system->graph());
        systemsOrder.push(system.get());
    }

    // Determine the sequential order of systems
    for (auto it = systemsOrder.begin(); it != systemsOrder.end();) {
        bool hasSwap = false;
        for (const auto dependencyID : (*it)->dependencies()) {
            const auto dependencyExist = [dependencyID](const SystemPtr &system){ return system->typeID() == dependencyID; };
            // Swap `dependencyID` and `it` if `dependencyID` is not before `it`
            if (!std::any_of(systemsOrder.begin(), it, dependencyExist)) {
                auto dependencyIt = std::find_if(systemsOrder.begin(), systemsOrder.end(), dependencyExist);
                if (dependencyIt == systemsOrder.end())
                    throw std::logic_error("ECS::SystemGraph::build: Dependency do not exist");
                std::iter_swap(it, dependencyIt);
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
        (*current)->task().precede((*next)->task());
        ++current;
        ++next;
    }
}

// template<kF::ECS::EntityRequirements EntityType>
// inline void kF::ECS::SystemGraph<EntityType>::build(Registry<EntityType> &registry)
// {
//     using SystemPair = std::pair<ASystem<EntityType> *, typename ASystem<EntityType>::Dependencies>;

//     if (_systems.size() == 0ul)
//         throw std::logic_error("ECS::SystemGraph::build: No system in graph");

//     // Clear graph and prepare order set
//     Core::Vector<SystemPair> systemsOrder;
//     systemsOrder.reserve(_systems.size());
//     _graph.clear();

//     // Collect data over each systems
//     for (auto &system : _systems) {
//         system->setup(registry);
//         system->task() = _graph.emplace(system->graph());
//         systemsOrder.push(std::make_pair(system.get(), system->dependencies())); // is std::make_pair useless ?
//         // systemsOrder.push(system.get(), system->dependencies());
//     }

//     std::cout << "Before" << std::endl;
//     for (const auto &system : systemsOrder) {
//         std::cout << system.first->typeID().name() << std::endl;
//     }
//     std::cout << std::endl;

//     // Determine the sequential order of systems
//     for (auto it = systemsOrder.begin(); it != systemsOrder.end();) {
//         bool hasSwap = false;
//         std::cout << "SYSTEM: " << it->first->typeID().name() << std::endl;
//         for (const auto dependencyID : it->second) {
//             std::cout << "Dependency: " << dependencyID.name() << std::endl;
//             const auto dependencyExist = [dependencyID](const SystemPair &pair){ return pair.first->typeID() == dependencyID; };
//             // Swap `dependencyID` and `it` if `dependencyID` is not before `it`
//             if (!std::any_of(systemsOrder.begin(), it, dependencyExist)) {
//                 auto dependencyIt = std::find_if(systemsOrder.begin(), systemsOrder.end(), dependencyExist);
//                 if (dependencyIt == systemsOrder.end())
//                     throw std::logic_error("ECS::SystemGraph::build: Dependency do not exist");
//                 std::cout << "SWAP:\n\tDependencyIt: " << dependencyIt->first->typeID().name() << "\n\tit: " << it->first->typeID().name() << std::endl;

// //                std::iter_swap(it, dependencyIt);

//                 std::swap(it->first, dependencyIt->first);
//                 it->second.swap(dependencyIt->second);

//                 hasSwap = true;
//                 break;
//             }
//         }
//         std::cout << std::endl;
//         if (!hasSwap)
//             ++it;
//     }

//     std::cout << "After" << std::endl;
//     for (const auto &system : systemsOrder) {
//         std::cout << system.first->typeID().name() << std::endl;
//     }

//     // Build the sequential graph
//     auto current = systemsOrder.begin();
//     auto next = current + 1;
//     const auto end = systemsOrder.end();
//     while (next != end) {
//         current->first->task().precede(next->first->task());
//         ++current;
//         ++next;
//     }
// }

template<kF::ECS::EntityRequirements EntityType>
inline void kF::ECS::SystemGraph<EntityType>::clear(void) noexcept
{
    _systems.clear();
    _graph.clear();
}