/**
 * @ Author: Léo Lhuile
 * @ Description: SparseEntitySet
 */

template<typename EntityType, EntityType PageSize>
inline bool kF::ECS::SparseEntitySet<EntityType, PageSize>::exists(const EntityType entity) const noexcept
{
    if (!_pages) [[unlikely]]
        return false;
    const auto page = PageIndex(index);
    const auto it = _pages.begin<false>() + page;

    return page < _pages.size<false>() && (*it) && (*it)[ElementIndex(index)] != NullIndex;
}

template<typename EntityType, EntityType PageSize>
inline typename kF::ECS::SparseEntitySet<EntityType, PageSize>::Index kF::ECS::SparseEntitySet<EntityType, PageSize>::add(const EntityType entity) noexcept_ndebug
{
    kFAssert(!exists(entity),
        throw std::logic_error("ECS::SparseEntitySet::add: Entity already exists"));

    // Add the entity to the flat set
    const auto index = _entityCount++;
    _flatset.push(entity);

    // Add the entity to the sparse set
    const auto page = PageIndex(entity);
    auto it = _pages.begin() + page;
    if (page >= _pages.size()) [[unlikely]] {
        const auto toInsert = page - _pages.size();
        _pages.insert(_pages.end(), toInsert, PagePtr());
        it = _pages.begin() + page;
        (*it) = std::make_unique<Page>(NullIndex);
    } else if (!(*it)) [[unlikely]] {
        (*it) = std::make_unique<Page>(NullIndex);
    }
    auto &target = (*it)[ElementIndex(entity)];
    target = index;
    return index;
}

template<typename EntityType, EntityType PageSize>
inline typename kF::ECS::SparseEntitySet<EntityType, PageSize>::Index kF::ECS::SparseEntitySet<EntityType, PageSize>::remove(const EntityType entity) noexcept_ndebug
{
    kFAssert(exists(entity),
        throw std::logic_error("ECS::SparseEntitySet::remove: Entity doesn't exists"));

    // Decrement entity count
    const auto lastIndex = --_entityCount;

    // Move the last entity the removed index and pop the last one
    const auto data = _flatset.data<false>(); // data<false> because we want to disable checks
    const auto lastEntity = data[lastIndex];
    data[index] = lastEntity;
    _flatset.erase(data + lastIndex); // Delete the last entity of the flat set

    // Change the sparse entity index of the last entity in the flat set
    atRef(lastEntity) = index;

    // Nullify the entity index in the sparse set
    auto &value = atRef(entity);
    value = NullIndex;

    return index;
}

template<typename EntityType, EntityType PageSize>
inline void kF::ECS::SparseEntitySet<EntityType, PageSize>::clear(void) noexcept
{
    _pages.clear();
    _flatset.clear();
    _entityCount = 0u;
}
