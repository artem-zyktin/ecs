#pragma once

#include <cstddef>
#include <concepts>
#include <memory>

namespace ecs
{

template<typename T, template<typename> typename A>
concept std_compatible_allocator = std::is_default_constructible_v<A<T>>
&& std::allocator_traits<A<T>>::is_always_equal::value
&& requires(A<T> a, std::size_t n, T * p)
{
	typename A<T>::value_type;
	requires std::same_as<typename A<T>::value_type, T>;
	{ A<T>{} } -> std::default_initializable;
	{ a.allocate(n) } noexcept -> std::same_as<T*>;
	{ a.deallocate(p, n) } noexcept;
};

} // namespace ecs
