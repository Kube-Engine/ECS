/**
 * @ Author: Léo Lhuile
 * @ Description: SparseIndexSet
 */

#pragma once

#include <vector>
#include <memory>

#include <Kube/Core/Utils.hpp>

namespace kF::ECS
{
    template<std::integral Type, std::size_t PageSize, Type Null>
    class SparseIndexSet;
}

/** @brief The sparse index set is a container which provide O(1) look-up time at the cost of
 *  non-efficient memory consumption */
template<std::integral Type, std::size_t PageSize, Type Null>
class kF::ECS::SparseIndexSet
{
public:
    using Page = Type[PageSize];
    using PagePtr = std::unique_ptr<Page>;

    /** @brief Default constructor. */
    SparseIndexSet(void) noexcept = default;

    /** @brief Default move constructor. */
    SparseIndexSet(SparseIndexSet &&other) noexcept = default;

    /** @brief Default destructor. */
    ~SparseIndexSet(void) noexcept = default;

    /** @brief Returns true if the page containing index exists */
    [[nodiscard]] bool exists(const std::size_t index) const noexcept;

    /** @brief Add a new value to the set */
    template<typename ...Args>
    void add(const std::size_t index, Args &&...args) noexcept;

    /** @brief Remove a value from the set and return it */
    [[nodiscard]] Type remove(const std::size_t index) noexcept_ndebug;

    /** @brief Access a given element of the set */
    [[nodiscard]] Type &at(const std::size_t index) const noexcept { return _pages[pageIndex(index)][elementIndex(index)]; }
    [[nodiscard]] const Type &at(const std::size_t index) const noexcept { return _pages[pageIndex(index)][elementIndex(index)]; }

    /** @brief Retreive the index of a page */
    [[nodiscard]] std::size_t pageIndex(const std::size_t index) const noexcept { return index / PageSize; }

    /** @brief Retreive the index of an element */
    [[nodiscard]] std::size_t elementIndex(const std::size_t index) const noexcept { return index % PageSize; }

private:
    std::vector<Page> _pages {};
};

#include "SparseIndexSet.ipp"
