#pragma once

#include "ecs/std_compotible_allocator_concept.h"
#include "ecs/default_allocator.h"
#include "ecs/component_id.h"
#include "ecs/component.h"

#include <cstdint>
#include <array>
#include <type_traits>

namespace ecs
{


struct component_locator
{
	using size_type = std::remove_cv_t<decltype(MAX_COMPONENT_ID)>;
	static constexpr size_type MAX_COUNT = MAX_COMPONENT_ID;

	component_locator() noexcept = default;
	~component_locator() noexcept;

	component_locator(const component_locator&) = delete;
	component_locator& operator=(const component_locator&) = delete;

	component_locator(component_locator&&) noexcept = default;
	component_locator& operator=(component_locator&&) noexcept = default;

	template<typename component_t, template<typename> typename allocator = default_allocator>
	requires std_compatible_allocator<component_t, allocator>
	component_t* reg() noexcept;

	template<typename component_t>
	component_t* get() const noexcept;

private:
	static component_id last_id_;

	template<typename component_t>
	static component_id& get_component_id_() noexcept;

	template<typename component_t>
	static component_id ensure_component_id_() noexcept;

	struct _erasure_storage
	{
		void* ptr = nullptr;
		void (*deleter)(void*) = nullptr;
	};

	std::array<_erasure_storage, MAX_COUNT> container_ = {};
};

} // namespace ecs

#include "ecs/component_locator_impl.hpp"
