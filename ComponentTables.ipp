/**
 * @ Author: Léo Lhuile
 * @ Description: ComponentTables
 */

template <typename EntityType>
template <typename Component>
inline void kF::ECS::ComponentTables<EntityType>::add(void) noexcept_ndebug
{
    using Table = ComponentTable<Component>;

    constexpr auto TypeSize = sizeof(Meta::Type);
    constexpr auto TableSize = sizeof(Table);

    kFAssert(tableExists<Component>(),
             throw std::logic_error("ECS::ComponentTables::add: Component table already added"));

    const auto oldTypeByteSize = _size * TypeSize;
    const auto oldTableByteSize = _size * TableSize;
    const auto newSize = _size + 1;
    const auto newTypeByteSize = newSize * TypeSize;
    const auto newTableByteSize = newSize * TableSize;
    const auto newByteSize = newTypeByteSize + newTableByteSize;
    auto tmp = std::make_unique<std::byte[]>(newByteSize);

    if (_size)
        [[likely]]
        {
            std::memcpy(tmp.get(), _data.get(), oldTypeByteSize);                                      // Copy old meta types
            std::memcpy(tmp.get() + newTypeByteSize, _data.get() + oldTypeByteSize, oldTableByteSize); // Copy old component tables
        }
    reinterpret_cast<Meta::Type *>(tmp.get())[_size] = Meta::Factory<Table>::Resolve(); // Add the new meta type
    new (tmp.get() + newByteSize - TableSize) Table();                                  // Add the new component table
    _data.swap(tmp);                                                                    // Exchange the temporary data with the internal one (will destroy current at end of scope)
    _size = newSize;                                                                    // Set the new size
}

template <typename EntityType>
inline bool kF::ECS::ComponentTables<EntityType>::tableExists(const Meta::Type type) const noexcept
{
    auto it = getTypeData();
    const auto end = it + _size;

    while (it != end)
    {
        if (*it != type)
            [[likely]]++ it;
        else
            [[unlikely]] return true;
    }
    return false;
}

template <typename EntityType>
template <typename Component>
inline const kF::ECS::ComponentTable<Component> &kF::ECS::ComponentTables<EntityType>::getTable(void) const noexcept_ndebug
{
    using Table = ComponentTable<Component>;

    constexpr auto TypeSize = sizeof(Meta::Type);
    constexpr auto TableSize = sizeof(Table);

    const auto type = Meta::Factory<Component>::Resolve();
    const auto begin = getTypeData();
    const auto end = begin + _size;
    auto it = begin;

    while (it != end)
    {
        if (*it != type)
            [[likely]]++ it;
        else
            [[unlikely]]
            {
                const auto pos = std::distance(begin, it);
                return *reinterpret_cast<Table *>(_data.get() + _size * TypeSize + pos * TableSize);
            }
    }
    kFDebugThrow(std::logic_error("ECS::ComponentTable::GetTable: Table doesn't exists"));
}

template <typename EntityType>
inline void kF::ECS::ComponentTables<EntityType>::clear(void)
{
    _data.reset();
}
