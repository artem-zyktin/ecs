#pragma once

#include "ecs/entity_id.h"

#include <type_traits>
#include <iterator>
#include <limits>
#include <array>

namespace ecs
{

template<typename T>
struct abstract_component
{
	static_assert(std::is_nothrow_move_constructible_v<T>);
	static_assert(std::is_nothrow_move_assignable_v<T>);

	using value_type		 = std::remove_cvref_t<T>;
	using ref_type			 = value_type &;
	using const_ref_type	 = value_type const &;
	using pointer_type		 = value_type *;
	using const_pointer_type = value_type const *;
	
	using size_type = uint32_t;
	using index_type = uint32_t;

	using difference_type = std::ptrdiff_t;

	using iterator = pointer_type;
	using const_iterator = const_pointer_type;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static constexpr size_type MAX_SIZE = static_cast<size_type>(MAX_ENTITY_COUNT);

	virtual ~abstract_component() noexcept;

	template<typename... arg_t>
	pointer_type emplace(entity_id id, arg_t&&... arg) noexcept
	requires std::is_nothrow_constructible_v<value_type, arg_t...>;

	pointer_type set(entity_id id, value_type&& value) noexcept;

	pointer_type set(entity_id id, const value_type& value) noexcept
	requires std::is_nothrow_copy_constructible_v<value_type>;

	const_pointer_type get(entity_id id) const noexcept;
	pointer_type get(entity_id id) noexcept;

	void remove(entity_id id) noexcept;

	bool has(entity_id id, const_pointer_type& out) const noexcept;
	bool has(entity_id id, pointer_type& out) noexcept;
	bool has(entity_id id) noexcept;

	inline size_type size() const noexcept { return size_; }
	inline bool empty() const noexcept { return size_ == 0; }

	entity_id get_id(index_type idx) const noexcept;
	
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

protected:

	abstract_component() noexcept;

	abstract_component(abstract_component&& other) noexcept = delete;
	abstract_component& operator=(abstract_component&& other) noexcept = delete;

	abstract_component(const abstract_component& other) noexcept = delete;
	abstract_component& operator=(const abstract_component& other) noexcept = delete;

	static constexpr size_type INVALID_INDEX = std::numeric_limits<size_type>::max();

	union _proxy_storage
	{
		value_type data;
		_proxy_storage() noexcept {}
		~_proxy_storage() noexcept {}
	};

	using container_t = std::array<_proxy_storage, MAX_SIZE>;
	using index_to_entity_t = std::array<entity_id, MAX_SIZE>;
	using entity_to_index_t = std::array<index_type, MAX_ENTITY_COUNT>;

	container_t container_ = {};
	index_to_entity_t id_of_index_ = {};
	entity_to_index_t index_of_id_ = {};

	size_type size_ = 0;

	static bool index_is_valid_(index_type idx) noexcept;
	static bool entity_id_is_valid_(entity_id id) noexcept;

	const_pointer_type get_(index_type idx) const noexcept;
	pointer_type get_(index_type idx) noexcept;
};

} // namespace ecs

#include "ecs/abstract_component.hpp"
