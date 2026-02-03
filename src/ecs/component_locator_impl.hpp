#pragma once

#include "ecs/component_locator.h"

namespace ecs
{

template<typename T, template<typename> typename Allocator>
requires std_compatible_allocator<component<T>, Allocator>
inline component<T>* component_locator::reg() noexcept
{
	using component_t = component<T>;
	using allocator_t = Allocator<component_t>;

	auto idx = ensure_component_id_<component_t>();

	if (idx == INVALID_COMPONENT_ID)
	{
		return nullptr;
	}

	if (components_[idx].obj != nullptr)
	{
		return static_cast<component_t*>(components_[idx].obj);
	}

	allocator_t allocator {};
	component_t* ptr = allocator.allocate(1);

	if (!ptr)
	{
		return nullptr;
	}
		
	ptr = std::construct_at(ptr);

	storage_t storage =
	{
		static_cast<void*>(ptr),
		+[](void* obj)
		{
			auto* p = static_cast<component_t*>(obj);
			std::destroy_at(p);
			allocator_t alloc{};
			alloc.deallocate(p, 1);
		}
	};

	components_[idx] = storage;

	return ptr;
}

template<typename T, template<typename> typename Allocator>
requires std_compatible_allocator<component<T>, Allocator>
inline component<T>* component_locator::get() const noexcept
{
	using component_t = component<T>;

	auto idx = get_existing_component_id_<component_t>();

	if (idx == INVALID_COMPONENT_ID)
	{
		return nullptr;
	}

	return static_cast<component_t*>(components_[idx].obj);
}

template<typename C>
inline component_id& component_locator::type_id_storage_() const noexcept
{
	static component_id id = INVALID_COMPONENT_ID;
	return id;
}

template<typename C>
inline component_id component_locator::ensure_component_id_() const noexcept
{
	auto& id = type_id_storage_<C>();

	if (id != INVALID_COMPONENT_ID)
	{
		return id;
	}

	if (next_component_id_ >= MAX_COMPONENT_COUNT)
	{
		return INVALID_COMPONENT_ID;
	}

	auto new_id = next_component_id_++;
	id = new_id;

	return id;
}

template<typename C>
inline component_id component_locator::get_existing_component_id_() const noexcept
{
	return type_id_storage_<C>();
}

} // namespace ecs
