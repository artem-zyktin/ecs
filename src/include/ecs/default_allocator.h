#pragma once

#include <cstddef>

namespace ecs
{

template<typename T>
struct default_allocator
{
	using value_type = T;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;

	value_type* allocate(size_t n) noexcept;
	void deallocate(value_type* p, size_t n) noexcept;
};

} // namespace ecs

#include "ecs/default_allocator.hpp"
