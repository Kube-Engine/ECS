/**
 * @ Author: Léos Julien
 * @ Description: An opaque system
 */
#pragma once // What is that ?

#include <Kube/Flow/Graph.hpp> // Why not including ?
#include <Kube/Flow/Task.hpp>

#include <vector>

namespace Kf::ECS
{
    template <typename EntityType>
    class ASystem;
}

/** @brief An opaque system */
template <typename EntityType>
class Kf::ECS::ASystem
{
public:
    /** @brief Construct a new system using a TypeID */
    ASystem(const std::type_info typeID) : _typeID(typeID){};

    /** @brief Destruct the system */
    virtual ~ASystem<EntityType>(void){}; // What <EntityType> is for ?

    /** @brief Setup the system */
    void setup(Registry<EntityType> &registry) = 0;

    /** @brief Get dependecies of the system */
    std::vector<std::type_info> dependencies(void) = 0;

    /** @brief Get system's internal Graph */
    Kf::Flow::Graph &graph(void) { return _graph; };

    /** @brief Get system's internal Graph */
    const Kf::Flow::Graph &graph(void) const { return _graph; }; // Is the first const not useless ? Found on SO: 'It makes no sense to return a const T because it is a copy anyway.'

    /** @brief Get system's internal Task */
    Kf::Flow::Task &task(void) { return _node; };

    /** @brief Get system's internal Task */
    const Kf::Flow::Task &task(void) const { return _node; };

    /** @brief Get system's TypeID */
    const std::type_info typeID(void) const { return _typeID; };

private:
    Kf::Flow::Graph _graph;
    Kf::Flow::Task _node;
    const std::type_info _typeID;
}