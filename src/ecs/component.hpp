#pragma once

#include <array>
#include <type_traits>
#include <iterator>
#include <cstdint>
#include <limits>

#include "ecs/entity.h"

namespace ecs
{

template<typename T>
class component
{
	static_assert(std::is_nothrow_move_constructible_v<T>);
	static_assert(std::is_nothrow_move_assignable_v<T>);
public:
	using value_type = std::remove_cvref_t<T>;
	using pointer_type = value_type *;
	using const_pointer_type = value_type const *;

	using iterator = pointer_type;
	using const_iterator = const_pointer_type;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	using size_type = uint32_t;
	using index_type = uint32_t;

	using difference_type = std::ptrdiff_t;

	static constexpr size_type MAX_SIZE = MAX_ENTITY_COUNT;
	static constexpr index_type INVALID_INDEX = std::numeric_limits<index_type>::max();

	component() noexcept;
	~component() noexcept;

	template<typename... arg_t>
	pointer_type emplace(entity_id id, arg_t&&... arg) noexcept requires std::is_nothrow_constructible_v<T, arg_t...>;

	pointer_type put(entity_id id, value_type&& value) noexcept;
	pointer_type set(entity_id id, value_type&& value) noexcept;

	pointer_type get(entity_id id) noexcept;
	const_pointer_type get(entity_id id) const noexcept;

	bool has(entity_id id) const noexcept;

	void remove(entity_id id) noexcept;

	size_type size() const noexcept { return size_; }

	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator begin() const noexcept;
	const_iterator end() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;
	reverse_iterator rbegin() noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;

private:
	union lazy_storage_
	{
		value_type data;
		lazy_storage_() {}
		~lazy_storage_() {}
	};

	using dense_t = std::array<lazy_storage_, MAX_SIZE>;
	using dense_entity_t = std::array<entity_id, MAX_SIZE>;
	using sparse_t = std::array<index_type, MAX_ENTITY_COUNT>;

	dense_t dense_ = {};
	dense_entity_t dense_entity_ = {};
	sparse_t sparse_ = {};

	size_type size_ = 0;
};

#include "component_impl.hpp"

} // namespace ecs
