/**
 * @ Author: Léo Lhuile
 * @ Description: SparseSet
 */

#pragma once

#include <vector>
#include <memory>

namespace kF::ECS
{
    template<typename Type, std::size_t PageSize>
    class SparseSet;
}

template<typename Type, std::size_t PageSize>
class kF::ECS::SparseSet
{
public:
    /** @brief Default constructor. */
    SparseSet() = default;

    /** @brief Default move constructor. */
    SparseSet(SparseSet &&) = default;

    /** @brief Default destructor. */
    ~SparseSet() = default;

    bool pageExists(const std::size_t index) const;

    template<typename ...Args>
    void add(const std::size_t index, Args &&...args);

    void remove(const std::size_t index);

    [[nodiscard]] Type &at(const std::size_t index) const;
    [[nodiscard]] const Type &at(const std::size_t index) const;

private:
    std::vector<std::unique_ptr<Type[PageSize]>> _pages;
};

#include "SparseSet.ipp"
