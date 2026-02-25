#pragma once

#include <type_traits>
#include <concepts>

namespace ecs
{

template<template <typename> typename A, typename T>
concept allocator_concept = requires (A<T> a, T* p, size_t n)
{
	typename A<T>::value_type;
	requires std::same_as<typename A<T>::value_type, T>;

	{ a.allocate(n) } -> std::same_as<T*>;
	noexcept( a.allocate(n) );

	{ a.deallocate(p, n) };
	noexcept( a.deallocate(p, n) );
}
&& std::is_empty_v<A<T>>;

} // namespace ecs
