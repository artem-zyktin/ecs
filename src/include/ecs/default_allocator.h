#pragma once

#include "ecs/std_compotible_allocator_concept.h"

#include <cstddef>
#include <type_traits>

namespace ecs
{

template<typename T>
struct default_allocator
{
	using value_type   = T;
	using pointer_type = value_type *;
	using size_type    = std::size_t;

	pointer_type allocate(size_type n) noexcept;
	void deallocate(pointer_type p, size_type n) noexcept;
};

} // namespace ecs

#include "ecs/default_allocator_impl.hpp"
