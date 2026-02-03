#pragma once

#include "ecs/component_id.h"
#include "ecs/component.hpp"
#include "ecs/std_compotible_allocator_concept.hpp"
#include "ecs/default_allocator.hpp"

#include <cstdint>
#include <limits>
#include <type_traits>
#include <array>
#include <atomic>

namespace ecs
{

class component_locator
{
public:
	using size_type = uint32_t;

	static constexpr size_type MAX_COMPONENT_COUNT = size_type(MAX_COMPONENT_ID);

	component_locator() noexcept = default;
	~component_locator();

	component_locator(const component_locator&) = delete;
	component_locator& operator=(const component_locator&) = delete;
	component_locator(component_locator&&) = delete;
	component_locator& operator=(component_locator&&) = delete;

	/// <summary>
	/// Not thread safe - not register components in parallel
	/// </summary>
	template<typename T, template<typename> typename Allocator = default_allocator>
	requires std_compatible_allocator<component<T>, Allocator>
	component<T>* reg() noexcept;

	template<typename T, template<typename> typename Allocator = default_allocator>
	requires std_compatible_allocator<component<T>, Allocator>
	component<T>* get() const noexcept;

private:

	struct storage_t
	{
		void* obj = nullptr;
		void (*destroy)(void*) = nullptr;
	};

	using component_storage_t = std::array<storage_t, MAX_COMPONENT_COUNT>;

	mutable component_id next_component_id_ = {0};
	component_storage_t components_ = {};

	template<typename C>
	component_id& type_id_storage_() const noexcept;

	template<typename C>
	component_id ensure_component_id_() const noexcept;

	template<typename C>
	component_id get_existing_component_id_() const noexcept;
};

} // namaspace ecs

#include "component_locator_impl.hpp"
