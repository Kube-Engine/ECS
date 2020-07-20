/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Instance
 */

#include "Instance.hpp"

using namespace kF;

ECS::Instance::~Instance(void)
{
    for (auto &component : _components) {
        reinterpret_cast<IComponent *>(&component)->~IComponent();
    }
    _components.clear();
}