/**
 * @ Author: Léo Lhuile
 * @ Description: SparseEntitySet
 */

#include <stdexcept>

template<typename EntityType, EntityType PageSize>
inline bool kF::ECS::SparseEntitySet<EntityType, PageSize>::exists(const EntityType entity) const noexcept
{
    if (_pages.empty()) [[unlikely]]
        return false;

    const auto page = PageIndex(entity);
    const auto it = _pages.begin() + page;

    return page < _pages.size() && (*it) && (*it)[ElementIndex(entity)] != NullIndex;
}

template<typename EntityType, EntityType PageSize>
inline typename kF::ECS::SparseEntitySet<EntityType, PageSize>::Index
    kF::ECS::SparseEntitySet<EntityType, PageSize>::add(const EntityType entity) noexcept_ndebug
{
    kFAssert(!exists(entity),
        throw std::logic_error("ECS::SparseEntitySet::add: Entity already exists"));

    // Add the entity to the flat set
    const auto index = _flatset.size();
    _flatset.push(entity);

    // Add the entity to the sparse set
    const auto page = PageIndex(entity);
    auto it = _pages.begin() + page;
    if (page >= _pages.size()) [[unlikely]] {
        const auto toInsert = 1 + page - _pages.size();
        _pages.insert(_pages.end(), toInsert);
        it = _pages.begin() + page;
        *it = MakePage();
    } else if (!*it) [[unlikely]] {
        *it = MakePage();
    }
    (*it)[ElementIndex(entity)] = index;
    return index;
}

template<typename EntityType, EntityType PageSize>
inline typename kF::ECS::SparseEntitySet<EntityType, PageSize>::Index
    kF::ECS::SparseEntitySet<EntityType, PageSize>::remove(const EntityType entity) noexcept_ndebug
{
    kFAssert(exists(entity),
        throw std::logic_error("ECS::SparseEntitySet::remove: Entity doesn't exists"));

    // Get index of entity to remove
    const auto index = at(entity);

    // Move the last entity the removed index and pop the last one
    const auto lastEntity = _flatset.back();
    _flatset.pop();
    _flatset.at(index) = lastEntity;

    // Change the sparse entity index of the removed and the last entity in the flat set
    atRef(lastEntity) = index;
    atRef(entity) = NullIndex;

    return index;
}

template<typename EntityType, EntityType PageSize>
inline void kF::ECS::SparseEntitySet<EntityType, PageSize>::clear(void) noexcept
{
    _pages.clear();
    _flatset.clear();
}

template<typename EntityType, EntityType PageSize>
inline typename kF::ECS::SparseEntitySet<EntityType, PageSize>::Page
    kF::ECS::SparseEntitySet<EntityType, PageSize>::MakePage(void) noexcept_ndebug
{
    Index * const data = reinterpret_cast<Index *>(
        std::aligned_alloc(std::max(alignof(Index), alignof(std::max_align_t)), sizeof(Index) * PageSize)
    );

    kFAssert(data,
        throw std::runtime_error("ECS::SparseEntitySet::MakePage: Invalid aligned_alloc"));
    std::uninitialized_default_construct_n(data, PageSize);
    return Page(data);
}