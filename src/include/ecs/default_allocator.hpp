#pragma once

#include "ecs/default_allocator.h"

#include <new>

namespace ecs
{

template<typename T>
inline default_allocator<T>::value_type* default_allocator<T>::allocate(size_t n) noexcept
{
	void* p = ::operator new (n*sizeof(T), std::align_val_t{ alignof(T) }, std::nothrow);
	return static_cast<value_type*>(p);
}

template<typename T>
inline void default_allocator<T>::deallocate(value_type* p, size_t n) noexcept
{
	::operator delete(static_cast<void*>(p), std::align_val_t{ alignof(T) }, std::nothrow);
}

} // namespace ecs
