#pragma once

#include <algorithm>
#include <memory>
#include <ranges>

#include "ecs/component.hpp"

namespace ecs
{

template<typename T>
component<T>::component()
{
	std::fill(dense_entity_.begin(), dense_entity_.end(), INVALID_ENTITY_ID);
	std::fill(sparse_.begin(), sparse_.end(), INVALID_INDEX);
}

template<typename T>
inline component<T>::~component() noexcept
{
	if constexpr (!std::is_trivially_destructible_v<value_type>)
	{
		for (size_type i = 0; i < size_; ++i)
		{
			std::destroy_at(std::addressof(dense_[i].data));
		}
	}
}

template<typename T>
template<typename... arg_t>
component<T>::pointer_type component<T>::emplace(entity_id id, arg_t&& ...arg) noexcept
requires std::is_nothrow_constructible_v<T, arg_t...>
{
	if (id == INVALID_ENTITY_ID || id >= MAX_ENTITY_COUNT)
	{
		return nullptr;
	}

	pointer_type ptr = nullptr;
	if (!has(id))
	{
		if (size_ >= MAX_SIZE)
		{
			return nullptr;
		}

		index_type idx = size_;
		ptr = std::construct_at(std::addressof(dense_[idx].data), std::forward<arg_t>(arg)...);

		dense_entity_[idx] = id;
		sparse_[id] = idx;

		++size_;
	}
	else
	{
		ptr = set(id, value_type(std::forward<arg_t>(arg)...));
	}

	return ptr;
}



template<typename T>
inline component<T>::pointer_type component<T>::put(entity_id id, value_type&& value) noexcept
{
	if (id == INVALID_ENTITY_ID || id >= MAX_ENTITY_COUNT)
	{
		return nullptr;
	}

	if (has(id))
	{
		return get(id);
	}

	if (size_ >= MAX_SIZE)
	{
		return nullptr;
	}

	index_type idx = size_;

	pointer_type ptr = std::construct_at(std::addressof(dense_[idx].data), std::move(value));

	dense_entity_[idx] = id;
	sparse_[id] = idx;

	++size_;

	return ptr;
}

template<typename T>
inline component<T>::pointer_type component<T>::set(entity_id id, value_type&& value) noexcept
{
	if (id == INVALID_ENTITY_ID || id >= MAX_ENTITY_COUNT)
	{
		return nullptr;
	}

	index_type idx = sparse_[id];

	if (idx == INVALID_INDEX)
	{
		return nullptr;
	}

	pointer_type ptr = std::addressof(dense_[idx].data);
	*ptr = std::move(value);

	return ptr;
}

template<typename T>
inline component<T>::pointer_type component<T>::get(entity_id id) noexcept
{
	if (!has(id))
	{
		return nullptr;
	}

	return std::addressof(dense_[sparse_[id]].data);
}

template<typename T>
inline component<T>::const_pointer_type component<T>::get(entity_id id) const noexcept
{
	if (!has(id))
	{
		return nullptr;
	}

	return std::addressof(dense_[sparse_[id]].data);
}

template<typename T>
inline bool component<T>::has(entity_id id) const noexcept
{
	if (id == INVALID_ENTITY_ID || id >= MAX_ENTITY_COUNT)
	{
		return false;
	}

	index_type idx = sparse_[id];
	return idx != INVALID_INDEX && dense_entity_[idx] == id;
}

template<typename T>
inline void component<T>::remove(entity_id id) noexcept
{
	if (!has(id))
	{
		return;
	}

	index_type idx = sparse_[id];
	index_type last = size_ - 1;

	if (idx != last)
	{
		dense_[idx].data = std::move(dense_[last].data);

		dense_entity_[idx] = dense_entity_[last];
		sparse_[dense_entity_[idx]] = idx;
	}

	std::destroy_at(std::addressof(dense_[last].data));
	dense_entity_[last] = INVALID_ENTITY_ID;
	sparse_[id] = INVALID_INDEX;

	--size_;
}

template<typename T>
inline component<T>::iterator component<T>::begin() noexcept
{	
	return std::addressof(dense_[0].data);
}

template<typename T>
inline component<T>::iterator component<T>::end() noexcept
{
	return begin() + size_;
}

template<typename T>
inline component<T>::const_iterator component<T>::begin() const noexcept
{
	return std::addressof(dense_[0].data);
}

template<typename T>
inline component<T>::const_iterator component<T>::end() const noexcept
{
	return begin() + size_;
}

template<typename T>
inline component<T>::const_iterator component<T>::cbegin() const noexcept
{
	return std::addressof(dense_[0].data);
}

template<typename T>
inline component<T>::const_iterator component<T>::cend() const noexcept
{
	return cbegin() + size_;
}

template<typename T>
inline component<T>::reverse_iterator component<T>::rbegin() noexcept
{
	return std::reverse_iterator(end());
}

template<typename T>
inline component<T>::reverse_iterator component<T>::rend() noexcept
{
	return std::reverse_iterator(begin());
}

template<typename T>
inline component<T>::const_reverse_iterator component<T>::rbegin() const noexcept
{
	return std::reverse_iterator(end());
}

template<typename T>
inline component<T>::const_reverse_iterator component<T>::rend() const noexcept
{
	return std::reverse_iterator(begin());
}

template<typename T>
inline component<T>::const_reverse_iterator component<T>::crbegin() const noexcept
{
	return std::reverse_iterator(cend());
}

template<typename T>
inline component<T>::const_reverse_iterator component<T>::crend() const noexcept
{
	return std::reverse_iterator(cbegin());
}



} // namespace ecs
