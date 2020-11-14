/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTables
 */

template<kF::ECS::EntityRequirements EntityType>
template<typename Component>
void kF::ECS::ComponentTables<EntityType>::add(void) noexcept_ndebug
{
    using Table = ComponentTable<Component, EntityType>;

    kFAssert(!tableExists<Component>(),
        throw std::logic_error("ECS::ComponentTables::add: Component table already added"));

    const auto opaqueTable = GetOpaqueComponentTable<Component, EntityType>();

    _opaqueTables.push(opaqueTable);
    _removeFuncs.push(opaqueTable->removeFunc);
    new (&_tables.push()) Table();
}

template<kF::ECS::EntityRequirements EntityType>
inline bool kF::ECS::ComponentTables<EntityType>::tableExists(const OpaqueTable opaqueTable) const noexcept
{
    for (const auto it : _opaqueTables) {
        if (it == opaqueTable) [[unlikely]]
            return true;
    }
    return false;
}

template<kF::ECS::EntityRequirements EntityType>
template<typename Component>
inline const kF::ECS::ComponentTable<Component, EntityType> &kF::ECS::ComponentTables<EntityType>::getTable(void) const noexcept_ndebug
{
    using Table = ComponentTable<Component, EntityType>;

    const auto opaqueTable = GetOpaqueComponentTable<Component, EntityType>();

    for (auto i = 0ul; const auto it : _opaqueTables) {
        if (it == opaqueTable) [[unlikely]]
            return reinterpret_cast<const Table &>(_tables.at(i));
        ++i;
    }
    kFDebugThrow(std::logic_error("ECS::ComponentTable::GetTable: Table doesn't exists"));
}

template<kF::ECS::EntityRequirements EntityType>
void kF::ECS::ComponentTables<EntityType>::removeEntity(const EntityType entity)
{
    for (auto i = 0ul; const auto removeFunc : _removeFuncs) {
        (*removeFunc)(&_tables.at(i), entity);
        ++i;
    }
}

template<kF::ECS::EntityRequirements EntityType>
inline void kF::ECS::ComponentTables<EntityType>::clear(void)
{
    for (auto i = 0ul; const auto it : _opaqueTables) {
        (*it->destroyFunc)(&_tables.at(i));
        ++i;
    }
    _opaqueTables.clear();
    _removeFuncs.clear();
    _tables.clear();
}
