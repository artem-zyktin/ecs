#pragma once

#include <cstddef>
#include <concepts>

namespace ecs
{

template<typename T, template<typename> typename A>
concept std_compatible_allocator = std::is_default_constructible_v<A<T>>
&& requires(A<T> a, std::size_t n, T * p)
{
	typename A<T>::value_type;
	requires std::same_as<typename A<T>::value_type, T>;
	{ a.allocate(n) } -> std::same_as<T*>;
	{ a.deallocate(p, n) };
};

} // namespace ecs
