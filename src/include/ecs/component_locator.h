#pragma once

#include "ecs/component_concept.h"
#include "ecs/allocator_concept.h"
#include "ecs/default_allocator.h"

#include <cstdint>
#include <array>
#include <limits>

namespace ecs
{

struct component_locator
{
	using size_type = uint32_t;

	static constexpr size_type MAX_SIZE = 512;

	component_locator() noexcept = default;
	~component_locator() noexcept;

	component_locator(const component_locator&) = delete;
	component_locator& operator=(const component_locator&) = delete;

	template<ecs_component T, template<typename> typename Allocator = default_allocator>
	requires allocator_concept<Allocator, T>
	T* add() noexcept;

	template<ecs_component T>
	void remove() noexcept;

	template<ecs_component T>
	bool has() const noexcept;

	template<ecs_component T>
	bool has(T*& out) const noexcept;

	template<ecs_component T>
	T* get() const noexcept;

private:

	struct _type_erasure_storage
	{
		void* p = nullptr;
		void (*deleter)(void*) = nullptr;
	};

	using container_t = std::array <_type_erasure_storage, MAX_SIZE>;
	using component_index = uint32_t;

	static constexpr component_index INVALID_COMPONENT_INDEX_ = std::numeric_limits<component_index>::max();

	static inline component_index next_component_index_ = 0;
	container_t container_ = {};

	template<ecs_component T>
	component_index& type_index() noexcept;

	template<ecs_component T>
	component_index acquire_type_index() noexcept;
};

} // namespace ecs

#include "ecs/component_locator.hpp"
