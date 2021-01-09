/**
 * @ Author: Léo Lhuile
 * @ Description: SparseEntitySet
 */

#pragma once

#include <memory>
#include <array>
#include <cstdlib>

#include <Kube/Core/Assert.hpp>
#include <Kube/Core/Utils.hpp>
#include <Kube/Core/Vector.hpp>

#include "Base.hpp"

namespace kF::ECS
{
    template<EntityRequirements EntityType, EntityType PageSize>
    class SparseEntitySet;
}

/** @brief The sparse index set is a container which provide O(1) look-up time at the cost of
 *  non-efficient memory consumption */
template<kF::ECS::EntityRequirements EntityType, EntityType PageSize>
class kF::ECS::SparseEntitySet
{
public:
    /** @brief An index is the same size as an entity */
    using Index = EntityType;

    /** @brief Helper used to delete pages */
    struct PageDeleter
    {
        void operator()(Index *page) const noexcept { Core::Utils::AlignedFree(page); }
    };

    /** @brief Page containing indexes */
    using Page = std::unique_ptr<Index[], PageDeleter>;

    /** @brief A null index */
    static constexpr auto NullIndex = NullEntity<EntityType>;


    /** @brief Default constructor */
    SparseEntitySet(void) noexcept = default;

    /** @brief Default move constructor */
    SparseEntitySet(SparseEntitySet &&other) noexcept = default;

    /** @brief Default destructor */
    ~SparseEntitySet(void) noexcept = default;

    /** @brief Move assignment */
    SparseEntitySet &operator=(SparseEntitySet &&other) noexcept = default;


    /** @brief Returns true if the page containing index exists */
    [[nodiscard]] bool exists(const EntityType entity) const noexcept;

    /** @brief Add a new value to the set */
    Index add(const EntityType entity) noexcept_ndebug;

    /** @brief Remove a value from the set and return it
     *  @return The position of the destroyed entity in the flat set */
    Index remove(const EntityType entity) noexcept_ndebug;

    /** @brief Clear the sparse set */
    void clear(void) noexcept;


    /** @brief Access a given element of the set */
    [[nodiscard]] Index at(const EntityType entity) const noexcept { return _pages[PageIndex(entity)][ElementIndex(entity)]; }
    [[nodiscard]] Index &atRef(const EntityType entity) noexcept { return _pages[PageIndex(entity)][ElementIndex(entity)]; }


    /** @brief Get internal iterable flat set */
    [[nodiscard]] const Core::Vector<EntityType, EntityType> &flatset(void) const noexcept { return _flatset; }

    /** @brief Get the entity count */
    [[nodiscard]] EntityType entityCount(void) const noexcept { return _flatset.size(); }


    /** @brief Retreive the index of a page */
    [[nodiscard]] static inline EntityType PageIndex(const EntityType entity) noexcept { return entity / PageSize; }

    /** @brief Retreive the index of an element */
    [[nodiscard]] static inline EntityType ElementIndex(const EntityType entity) noexcept { return entity % PageSize; }

private: // The structure size will vary depending of EntityType, from 32 to 40 bytes
    Core::Vector<Page, std::uint32_t> _pages {};
    Core::Vector<EntityType, EntityType> _flatset {};

    /** @brief Make a new page */
    [[nodiscard]] static Page MakePage(void) noexcept_ndebug;
};

#include "SparseEntitySet.ipp"
