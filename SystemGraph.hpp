/**
 * @ Author: Léos Julien
 * @ Description: A Flow Graph of System(s)
 */

#pragma once

#include <vector>

#include <Kube/Flow/Graph.hpp>

#include "ASystem.hpp"

namespace kF::ECS
{
    template <typename EntityType>
    class SystemGraph;

    template <typename EntityType>
    using SystemPtr = std::unique_ptr<ASystem<EntityType>>;
}

/** @brief A Flow Graph of system(s) */
template <typename EntityType>
class alignas(32) kF::ECS::SystemGraph
{
public:
    /** @brief Construct the System Graph */
    SystemGraph(void) noexcept = default;

    /** @brief Destruct the System Graph */
    ~SystemGraph(void) = default;


    /** @brief Add a System to the Graph */
    template <typename System, typename... Args>
        requires    std::derived_from<System, ASystem<EntityType>> &&
                    std::constructible_from<System, Args...>
    System &add(Args &&... args) noexcept(nothrow_ndebug && nothrow_constructible(System, Args...));

    /** @brief Check if a system is registered */
    template <typename System> requires std::derived_from<System, ASystem<EntityType>>
    [[nodiscard]] bool exists(void) const noexcept;

    /** @brief Retrieve a System in the Graph */
    template <typename System> requires std::derived_from<System, ASystem<EntityType>>
    [[nodiscard]] System &get(void)
        { return const_cast<System &>(const_cast<const SystemGraph &>(*this).get<System>()); }

    /** @brief Retrieve a System in the Graph */
    template <typename System> requires std::derived_from<System, ASystem<EntityType>>
    [[nodiscard]] const System &get(void) const;


    /** @brief Setup and build the system graph according to internal system dependencies */
    void build(Registry<EntityType> &registry);

    /** @brief Clear all Systems from the Graph */
    void clear(void) noexcept;


    /** @brief Get system's internal Graph */
    [[nodiscard]] kF::Flow::Graph &graph(void) noexcept { return _graph; };

    /** @brief Get system's internal Graph */
    [[nodiscard]] const kF::Flow::Graph &graph(void) const noexcept { return _graph; };

private:
    kF::Flow::Graph _graph {};
    std::vector<kF::ECS::SystemPtr<EntityType>> _systems {};
};

static_assert(sizeof(kF::ECS::SystemGraph<kF::ECS::Entity>) == 32, "SystemGraph must take 32 bytes");
static_assert(sizeof(kF::ECS::SystemGraph<kF::ECS::LongEntity>) == 32, "SystemGraph must take 32 bytes");

#include "SystemGraph.ipp"