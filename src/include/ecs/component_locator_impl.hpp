#pragma once

#include "ecs/component_locator.h"

namespace ecs
{

template<typename T>
inline component_id& component_locator::get_component_id_() noexcept
{
    static component_id id = INVALID_COMPONENT_ID;
    return id;
}

template<typename component_t>
inline component_id component_locator::ensure_component_id_() noexcept
{
    component_id& id = get_component_id_<component_t>();

    if (id != INVALID_COMPONENT_ID)
    {
        return INVALID_COMPONENT_ID;
    }

    component_id new_id = last_id_;
    ++new_id;

    if (new_id >= MAX_COUNT)
    {
        return INVALID_COMPONENT_ID;
    }

    id = new_id;
    last_id_ = id;

    return id;
}

template<typename component_t, template<typename> typename allocator>
requires std_compatible_allocator<component_t, allocator>
inline component_t* component_locator::reg() noexcept
{
	using allocator_t = allocator<component_t>;

    component_id id = ensure_component_id_<component_t>();

    if (id == INVALID_COMPONENT_ID)
    {
        return nullptr;
    }

    allocator_t alloc = {};
    component_t* ptr = alloc.allocate(1);

    if (!ptr)
    {
        return nullptr;
    }

    ptr = std::construct_at(ptr);

    _erasure_storage storage =
    {
        static_cast<void*>(ptr),
        +[](void* p) // deleter
        {
            component_t* ptr = static_cast<component_t*>(p);
            std::destroy_at(ptr);

            allocator_t alloc = {};
            alloc.deallocate(ptr, 1);
        }
    };
    container_[id] = storage;

    return ptr;
}

template<typename component_t>
inline component_t* component_locator::get() const noexcept
{
    component_id id = get_component_id_<component_t>();

    if (id == INVALID_COMPONENT_ID)
    {
        return nullptr;
    }

    if (container_[id].ptr == nullptr)
    {
        return nullptr;
    }

    return static_cast<component_t*>(container_[id].ptr);
}

} // namespace ecs
