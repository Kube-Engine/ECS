/**
 * @ Author: Léo Lhuile
 * @ Description: SparseIndexSet
 */

template<std::integral Type, std::size_t PageSize, Type Null>
inline bool kF::ECS::SparseIndexSet<Type, PageSize, Null>::exists(const std::size_t index) const noexcept
{
    const auto page = pageIndex(index);
    const auto it = _pages.begin() + page;

    return page < _pages.size() && (*it) && (*it)[elementIndex(index)] != Null;
}

template<std::integral Type, std::size_t PageSize, Type Null>
template<typename ...Args>
inline void kF::ECS::SparseIndexSet<Type, PageSize, Null>::add(const std::size_t index, const Type value) noexcept
{
    const auto page = pageIndex(index);
    auto it = _pages.begin() + page;

    if (page >= _pages.size()) [[unlikely]] {
        const auto toInsert = page - _pages.size();
        _pages.insert(_pages.end(), toInsert, PagePtr());
        it = _pages.begin() + page;
        (*it) = std::make_unique<Page>(Null);
    } else if (!(*it)) [[unlikely]] {
        (*it) = std::make_unique<Page>(Null);
    }
    (*it)[elementIndex(index)] = value;
}

template<std::integral Type, std::size_t PageSize, Type Null>
inline Type kF::ECS::SparseIndexSet<Type, PageSize, Null>::remove(const std::size_t index) noexcept_ndebug
{
    kFAssert(exists(index),
        throw std::logic_error("ECS::SparseIndexSet::remove: Index doesn't exists"));
    auto &value = at(index);
    const auto lastValue = value;
    value = Null;
    return lastValue;
}
