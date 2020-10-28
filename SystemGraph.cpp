/**
 * @ Author: Léos Julien
 * @ Description: A Flow Graph of System(s)
 */

#include <algorithm>
#include <typeinfo>

using namespace kF;

template <typename System>
System &ECS::SystemGraph::get(void)
{
    auto systemTypeID = std::typeid(System)
    auto it = std::find_if(this._systems.begin(), this._systems.end(), [systemTypeID](ECS::SystemPtr& systemPtr){
        return systemPtr.typeID() == systemTypeID
    })

    if (it != this._systems.begin()) {
        return *it;
    } else {
        // what to do ?
    }
}