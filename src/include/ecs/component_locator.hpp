#pragma once

#include "ecs/component_locator.h"

namespace ecs
{

template<ecs_component T, template<typename> typename Allocator>
requires allocator_concept<Allocator, T>
inline T* component_locator::add() noexcept
{
	using allocator_t = Allocator<T>;

	component_index idx = acquire_type_index<T>();

	if (idx == INVALID_COMPONENT_INDEX_ || idx >= MAX_SIZE)
	{
		return nullptr;
	}

	T* p;
	if (container_[idx].p)
	{
		p = static_cast<T*>(container_[idx].p);
		std::destroy_at(p);
	}
	else
	{
		p = allocator_t{}.allocate(1);

		if (p == nullptr)
		{
			return nullptr;
		}
	}

	_type_erasure_storage storage {};

	storage.p = static_cast<void*>(std::construct_at(p));
	storage.deleter = [](void* ptr)
	{
		T* p = static_cast<T*>(ptr);
		std::destroy_at(p);
		allocator_t{}.deallocate(p, 1);
	};

	container_[idx] = storage;

	return static_cast<T*>(storage.p);
}

template<ecs_component T>
inline void component_locator::remove() noexcept
{
	component_index idx = type_index<T>();

	if (idx == INVALID_COMPONENT_INDEX_ || idx >= MAX_SIZE)
	{
		return;
	}

	auto& storage = container_[idx];

	if (!storage.p)
	{
		return;
	}

	storage.deleter(storage.p);

	storage.p = nullptr;
	storage.deleter = nullptr;
}

template<ecs_component T>
inline bool component_locator::has() const noexcept
{
	T* skip = nullptr;
	return has(skip);
}

template<ecs_component T>
inline bool component_locator::has(T*& out) const noexcept
{
	component_index idx = type_index<T>();

	if (idx == INVALID_COMPONENT_INDEX_ || idx >= MAX_SIZE)
	{
		out = nullptr;
		return false;
	}

	auto& storage = container_[idx];

	if (!storage.p)
	{
		out = nullptr;
		return false;
	}

	out = static_cast<T*>(storage.p);
	return true;
}

template<ecs_component T>
inline T* component_locator::get() const noexcept
{
	T* ptr = nullptr;
	if (has(ptr))
	{
		return ptr;
	}

	return nullptr;
}

template<ecs_component T>
inline component_locator::component_index& component_locator::type_index() const noexcept
{
	static component_index index = INVALID_COMPONENT_INDEX_;
	return index;
}

template<ecs_component T>
inline component_locator::component_index component_locator::acquire_type_index() noexcept
{
	component_index& index = type_index<T>();

	if (index != INVALID_COMPONENT_INDEX_)
		return index;

	if (next_component_index_ >= MAX_SIZE)
		return INVALID_COMPONENT_INDEX_;

	index = next_component_index_++;
	return index;
}

} // namespace ecs
