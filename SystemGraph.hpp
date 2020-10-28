/**
 * @ Author: Léos Julien
 * @ Description: A Flow Graph of System(s)
 */
#pragma once // What is that ?

#include <Kube/Flow/Graph.hpp> // Why not including ?

#include <vector>

#include "ASystem.hpp"

namespace Kf::ECS
{
    template <typename EntityType>
    class SystemGraph;

    template <typename EntityType>
    using SystemPtr = std::unique_ptr<ASystem<EntityType>>
} // namespace Kf::ECS

/** @brief A Flow Graph of system(s) */
template <typename EntityType>
class Kf::ECS::SystemGraph
{
public:
    /** @brief Construct the System Graph */
    SystemGraph(void){};

    /** @brief Destruct the System Graph */
    virtual ~SystemGraph(void){};

    /** @brief Add a System to the Graph */
    template <typename System, typename... Args>
    System &add<System, Args...>(Args &&... args); // Why Args ? What is '&&' ? Equals to '**' ? Implementation example ?

    /** @brief Setup and build the system graph according to internal system dependencies */
    void build(void);

    /** @brief Clear all Systems from the Graph */
    void clear(void);

    /** @brief Retrieve a System in the Graph */
    template <typename System>
    System &get<System>(void);

    /** @brief Retrieve a System in the Graph */
    template <typename System>
    const System &get<System>(void) const;

    /** @brief Get system's internal Graph */
    Kf::Flow::Graph &graph(void) { return _graph; };

    /** @brief Get system's internal Graph */
    const Kf::Flow::Graph &graph(void) const { return _graph; };

private:
    Kf::Flow::Graph _graph;
    std::vector<Kf::ECS::SystemPtr<EntityType>> _systems;
}