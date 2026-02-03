#include "ecs/default_allocator.hpp"

namespace ecs
{

template<typename T>
inline default_allocator<T>::pointer_type default_allocator<T>::allocate(size_type n)
{
	return static_cast<pointer_type>(::operator new(n*sizeof(value_type), std::align_val_t {alignof(value_type)}, std::nothrow));
}

template<typename T>
inline void default_allocator<T>::deallocate(pointer_type p, size_type n)
{
	::operator delete(p, n * sizeof(value_type), std::align_val_t {alignof(value_type)});
}

} // namespace ecs
