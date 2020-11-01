/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTables
 */

#include "ComponentTables.hpp"

template<typename EntityType>
template<typename Component>
inline void kF::ECS::ComponentTables<EntityType>::add(void)
{
    if (!tableExists<Component>()) {
        const auto oldData = _data.release();
        const auto sizeType = sizeof(Meta::Type::TypeID);
        const auto sizeTable = sizeof(ComponentTable<Component>);

        _size += 1;
        _data.reset(new std::byte[_size * sizeType + _size * sizeTable]);

        &(_data[_size * sizeType]) = std::move(&(oldData[_size * sizeType]));
        &(_data[_size * sizeType + (_size - 1) * sizeTable]) = std::move(ComponentTable<Component>());

        _data = std::move(oldData);
        &(_data[(_size - 1) * sizeType]) = Meta::Factory<Component>::Resolve();
    }
}

template<typename EntityType>
inline bool kF::ECS::ComponentTables<EntityType>::tableExists(const Meta::Type type) const noexcept
{
    const auto size = sizeof(Meta::Type::TypeID);

    for (std::size_t i = 0; i < _size; i += 1) {
        const auto &otherType = reinterpret_cast<Meta::Type::TypeID &>(_data[i * size]);

        if (type == otherType) {
            return true;
        }
    }
    return false;
}

template<typename EntityType>
template<typename Component>
inline const kF::ECS::ComponentTable<Component> &kF::ECS::ComponentTables<EntityType>::getTable<Component>(void) const noexcept_ndebug
{
    kFAssert(tableExists<Component>(),
        throw std::logic_error("ECS::ComponentTable::getTable: Table doesn't exists"));
    const auto type = Meta::Factory<Component>::Resolve();
    const auto sizeType = sizeof(Meta::Type::TypeID);
    const auto sizeTable = sizeof(ComponentTable<Component>);

    for (std::size_t i = 0; i < _size; i += 1) {
        const auto &otherType = reinterpret_cast<Meta::Type::TypeID &>(_data[i * sizeType]);

        if (type == otherType) {
            return reinterpret_cast<ComponentTable<Component> &>(_data[_size * sizeType + i * sizeTable]);
        }
    }
}

template<typename EntityType>
inline void kF::ECS::ComponentTables<EntityType>::clear(void)
{
    _data.reset({});
}
