#pragma once

#include "ecs/abstract_component.h"

#include <algorithm>

namespace ecs
{

template<typename T>
inline abstract_component<T>::abstract_component() noexcept
{
	std::ranges::fill(id_of_index_, INVALID_ENTITY_ID);
	std::ranges::fill(index_of_id_, INVALID_INDEX);
}

template<typename T>
inline abstract_component<T>::~abstract_component() noexcept
{
	std::destroy_n(get_(0), size_);
}

template<typename T>
inline abstract_component<T>::pointer_type abstract_component<T>::set(entity_id id, value_type&& value) noexcept
{
	if (!entity_id_is_valid_(id))
	{
		return nullptr;
	}

	index_type idx = index_of_id_[id];
	pointer_type ptr = nullptr;

	if (!index_is_valid_(idx))
	{
		if (size_ >= MAX_SIZE)
		{
			return nullptr;
		}

		idx = size_++;
		ptr = std::construct_at(get_(idx), std::move(value));

		index_of_id_[id] = idx;
		id_of_index_[idx] = id;
	}
	else
	{
		container_[idx].data = std::move(value);
		ptr = get_(idx);
	}

	return ptr;
}

template<typename T>
inline abstract_component<T>::pointer_type abstract_component<T>::set(entity_id id, const value_type& value) noexcept
requires std::is_nothrow_copy_constructible_v<value_type>
{
	if (!entity_id_is_valid_(id))
	{
		return nullptr;
	}

	index_type idx = index_of_id_[id];
	pointer_type ptr = nullptr;

	if (!index_is_valid_(idx))
	{
		if (size_ >= MAX_SIZE)
		{
			return nullptr;
		}

		idx = size_++;
		ptr = std::construct_at(get_(idx), value);

		index_of_id_[id] = idx;
		id_of_index_[idx] = id;
	}
	else
	{
		container_[idx].data = value;
		ptr = get_(idx);
	}

	return ptr;
}

template<typename T>
inline abstract_component<T>::const_pointer_type abstract_component<T>::get(entity_id id) const noexcept
{
	if (!entity_id_is_valid_(id))
	{
		return nullptr;
	}

	index_type idx = index_of_id_[id];

	if (!index_is_valid_(idx))
	{
		return nullptr;
	}

	return get_(idx);
}

template<typename T>
inline abstract_component<T>::pointer_type abstract_component<T>::get(entity_id id) noexcept
{
	if (!entity_id_is_valid_(id))
	{
		return nullptr;
	}

	index_type idx = index_of_id_[id];

	if (!index_is_valid_(idx))
	{
		return nullptr;
	}

	return get_(idx);
}

template<typename T>
inline void abstract_component<T>::remove(entity_id id) noexcept
{
	if (empty())
	{
		return;
	}

	if (!entity_id_is_valid_(id))
	{
		return;
	}

	index_type idx = index_of_id_[id];

	if (!index_is_valid_(idx))
	{
		return;
	}

	index_type last = size_ - 1;

	if (idx != last)
	{
		entity_id move = id_of_index_[last];

		*get_(idx) = std::move(*get_(last));

		id_of_index_[idx] = move;
		index_of_id_[move] = idx;
	}

	std::destroy_at(get_(last));

	id_of_index_[last] = INVALID_ENTITY_ID;
	index_of_id_[id] = INVALID_INDEX;

	--size_;
}

template<typename T>
inline bool abstract_component<T>::has(entity_id id, const_pointer_type& out) const noexcept
{
	if (empty())
	{
		out = nullptr;
		return false;
	}

	if (!entity_id_is_valid_(id))
	{
		out = nullptr;
		return false;
	}

	index_type idx = index_of_id_[id];

	if (!index_is_valid_(idx))
	{
		out = nullptr;
		return false;
	}

	out = get_(idx);
	return true;
}

template<typename T>
inline bool abstract_component<T>::has(entity_id id, pointer_type& out) noexcept
{
	if (empty())
	{
		out = nullptr;
		return false;
	}

	if (!entity_id_is_valid_(id))
	{
		out = nullptr;
		return false;
	}

	index_type idx = index_of_id_[id];

	if (!index_is_valid_(idx))
	{
		out = nullptr;
		return false;
	}

	out = get_(idx);
	return true;
}

template<typename T>
inline bool abstract_component<T>::has(entity_id id) noexcept
{
	if (empty())
	{
		return false;
	}

	if (!entity_id_is_valid_(id))
	{
		return false;
	}

	index_type idx = index_of_id_[id];

	if (!index_is_valid_(idx))
	{
		return false;
	}

	return true;
}

template<typename T>
inline entity_id abstract_component<T>::get_id(index_type idx) const noexcept
{
	if (!index_is_valid_(idx))
	{
		return INVALID_ENTITY_ID;
	}

	return id_of_index_[idx];
}

template<typename T>
inline abstract_component<T>::iterator abstract_component<T>::begin() noexcept
{
	return get_(0);
}

template<typename T>
inline abstract_component<T>::iterator abstract_component<T>::end() noexcept
{
	return get_(0) + size_;
}

template<typename T>
inline abstract_component<T>::const_iterator abstract_component<T>::begin() const noexcept
{
	return cbegin();
}

template<typename T>
inline abstract_component<T>::const_iterator abstract_component<T>::end() const noexcept
{
	return cend();
}

template<typename T>
inline abstract_component<T>::const_iterator abstract_component<T>::cbegin() const noexcept
{
	return get_(0);
}

template<typename T>
inline abstract_component<T>::const_iterator abstract_component<T>::cend() const noexcept
{
	return get_(0) + size_;
}

template<typename T>
inline abstract_component<T>::reverse_iterator abstract_component<T>::rbegin() noexcept
{
	return reverse_iterator(end());
}

template<typename T>
inline abstract_component<T>::reverse_iterator abstract_component<T>::rend() noexcept
{
	return reverse_iterator(begin());
}

template<typename T>
inline abstract_component<T>::const_reverse_iterator abstract_component<T>::rbegin() const noexcept
{
	return crbegin();
}

template<typename T>
inline abstract_component<T>::const_reverse_iterator abstract_component<T>::rend() const noexcept
{
	return crend();
}

template<typename T>
inline abstract_component<T>::const_reverse_iterator abstract_component<T>::crbegin() const noexcept
{
	return reverse_iterator(cend());
}

template<typename T>
inline abstract_component<T>::const_reverse_iterator abstract_component<T>::crend() const noexcept
{
	return reverse_iterator(cbegin());
}

template<typename T>
inline bool abstract_component<T>::index_is_valid_(index_type idx) noexcept
{
	return idx != INVALID_INDEX && idx < MAX_SIZE;
}

template<typename T>
inline bool abstract_component<T>::entity_id_is_valid_(entity_id id) noexcept
{
	return id != INVALID_ENTITY_ID && id < MAX_ENTITY_COUNT;
}

template<typename T>
inline abstract_component<T>::const_pointer_type abstract_component<T>::get_(index_type idx) const noexcept
{
	return std::addressof(container_[idx].data);
}

template<typename T>
inline abstract_component<T>::pointer_type abstract_component<T>::get_(index_type idx) noexcept
{
	return std::addressof(container_[idx].data);
}

template<typename T>
template<typename... arg_t>
inline abstract_component<T>::pointer_type abstract_component<T>::emplace(entity_id id, arg_t&&... arg) noexcept
requires std::is_nothrow_constructible_v<abstract_component<T>::value_type, arg_t...>
{
	if (!entity_id_is_valid_(id))
	{
		return nullptr;
	}

	index_type idx = index_of_id_[id];
	pointer_type ptr = nullptr;

	if (index_is_valid_(idx))
	{
		container_[idx].data = value_type(std::forward<arg_t>(arg)...);
		ptr = get_(idx);
	}
	else
	{
		if (size_ >= MAX_SIZE)
		{
			return nullptr;
		}

		idx = size_++;

		ptr = std::construct_at(get_(idx), std::forward<arg_t>(arg)...);

		index_of_id_[id] = idx;
		id_of_index_[idx] = id;
	}

	return ptr;
}

} // namespace ecs
