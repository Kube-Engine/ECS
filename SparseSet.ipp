/**
 * @ Author: Léo Lhuile
 * @ Description: SparseSet
 */

template<typename Type, std::size_t PageSize>
bool kF::ECS::SparseSet<Type, PageSize>::pageExists(const std::size_t index) const
{
}

template<typename Type, std::size_t PageSize>
template<typename ...Args>
void kF::ECS::SparseSet<Type, PageSize>::add(const std::size_t index, Args &&...args)
{
//    _pages.push_back(std::forward<Args>(args)...);
}

template<typename Type, std::size_t PageSize>
void kF::ECS::SparseSet<Type, PageSize>::remove(const std::size_t index)
{
}
