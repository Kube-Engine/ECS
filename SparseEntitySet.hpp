/**
 * @ Author: Léo Lhuile
 * @ Description: SparseEntitySet
 */

#pragma once

#include <vector>
#include <memory>

#include <Kube/Core/Assert.hpp>
#include <Kube/Core/Utils.hpp>

namespace kF::ECS
{
    template<typename EntityType, EntityType PageSize>
    class SparseEntitySet;
}

/** @brief The sparse index set is a container which provide O(1) look-up time at the cost of
 *  non-efficient memory consumption */
template<typename EntityType, EntityType PageSize>
class alignas(24) kF::ECS::SparseEntitySet
{
public:
    /** @brief An index is the same size as an entity */
    using Index = EntityType;

    /** @brief A page indexes */
    using Page = Index[PageSize];

    /** @brief Unique pointer to page */
    using PagePtr = std::unique_ptr<Page>;

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
    [[nodiscard]] const Core::FlatVector<EntityType> &flatset(void) const noexcept { return _flatset; }

    /** @brief Get the entity count */
    [[nodiscard]] EntityType entityCount(void) const noexcept { return _entityCount; }


    /** @brief Retreive the index of a page */
    [[nodiscard]] static inline EntityType PageIndex(const EntityType entity) noexcept { return entity / PageSize; }

    /** @brief Retreive the index of an element */
    [[nodiscard]] static inline EntityType ElementIndex(const EntityType entity) noexcept { return entity % PageSize; }

private:
    Core::FlatVector<Page> _pages {};
    Core::FlatVector<EntityType> _flatset {};
    EntityType _entityCount { 0ul };
};

#include "SparseEntitySet.ipp"
