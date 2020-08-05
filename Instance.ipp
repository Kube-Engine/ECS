/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Instance
 */

template<typename Type>
kF::ECS::ComponentType kF::ECS::Instance::registerComponent(void) noexcept
{
    auto &component = *reinterpret_cast<Component<Type> *>(&_components.emplace_back());

    new (&component) Component<Type>();
    return Component<Type>::TypeRef() = _components.size() - 1;
}

template<typename Type>
kF::ECS::Component<Type> &kF::ECS::Instance::getComponent(void) noexcept_ndebug
{
    auto type = Component<Type>::Type();

    kFAssert(type != NullComponentType,
        throw std::runtime_error("ECS::Instance::getComponent: Couldn't find component"));
    return reinterpret_cast<Component<Type> &>(_components[type]);
}

template<typename ...Args>
kF::ECS::Entity kF::ECS::Instance::registerEntity(std::array<ComponentIndex, sizeof...(Args)> &array) noexcept
{
    static_assert(sizeof...(Args) != 0, "You can't register an Entity without components");

    auto type = getEntityType<Args...>();
    Entity entity;

    entity.type = type;
    if (!_freeEntities.empty()) {
        auto index = _freeEntities.back();
        _freeEntities.pop_back();
        _entities[index] = type;
        entity.index = index;
    } else {
        const EntityIndex size = _entities.size();
        _entities.emplace_back(type);
        entity.index = size;
    }
    addEntityComponents<0, std::tuple<Args...>>(entity.index, array);
    return entity;
}

template<bool ReleaseComponents>
void kF::ECS::Instance::releaseEntity(const EntityIndex index)
{
    auto type = _entities[index];

    _entities[index] = NullEntityType;
    _freeEntities.emplace_back(index);
    if constexpr (ReleaseComponents) {
        auto &desc = _descriptors[type];
        for (ComponentType i = 0, size = _components.size(); i < size; ++i) {
            if (!desc.components[i])
                continue;
            reinterpret_cast<IComponent *>(&_components[i])->releaseEntity(index);
        }
    }
}

template<typename ...Args>
kF::ECS::EntityType kF::ECS::Instance::getEntityType(void) noexcept
{
    static EntityType type = NullEntityType;

    if (type != NullEntityType)
        return type;
    EntityDescriptor descriptor;
    getEntityTypeImpl<0, std::tuple<Args...>>(descriptor);
    for (EntityType i = 0; const auto &desc : _descriptors) {
        if (desc != descriptor)
            ++i;
        else
            return type = i;
    }
    _descriptors.emplace_back(std::move(descriptor));
    return type = _descriptors.size() - 1;
}

template<std::size_t Index, typename Tuple>
void kF::ECS::Instance::getEntityTypeImpl(EntityDescriptor &descriptor) noexcept
{
    if constexpr (Index < std::tuple_size_v<Tuple>) {
        using Type = std::tuple_element_t<Index, Tuple>;

        auto type = Component<Type>::Type();
        if (type == NullComponentType)
            type = registerComponent<Type>();
        descriptor.components.set(type, true);
        getEntityTypeImpl<Index + 1, Tuple>(descriptor);
    }
}

template<std::size_t Index, typename Tuple>
void kF::ECS::Instance::addEntityComponents(const EntityIndex index, std::array<ComponentIndex, std::tuple_size_v<Tuple>> &array)
{
    if constexpr (Index < std::tuple_size_v<Tuple>) {
        using CurrentType = std::tuple_element_t<Index, Tuple>;

        array[Index] = getComponent<CurrentType>().add(index);
        addEntityComponents<Index + 1, Tuple>(index, array);
    }
}