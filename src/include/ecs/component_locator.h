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

/// component_locator is NOT thread safe during registration phase.
///
/// Two-phase usage model:
///
/// Phase 1: Registration (NOT thread-safe)
///     - All component types must be registered before use
///     - Registration must complete in a single thread
///     - No concurrent calls to reg() allowed
///
/// Phase 2: Runtime (thread-safe for reads)
///     - After registration phase, only get() operations are allowed
///     - Multiple threads can concurrently call get()
///     - No modifications to registered components allowed via locator
///
/// Ownership:
///     - component_locator owns all registered component instances
///     - Components are destroyed when locator is destroyed
///     - Component lifetime ends when locator lifetime ends
///
/// Usage example:
///     // Phase 1: Registration (single thread)
/// 	struct velocity : public component<vec3f> {};
/// 	struct position : public component<vec3f> {};
///     locator.reg<velocity>();
///     locator.reg<position>();
///     
///     // Phase 2: Runtime (multiple threads)
///     velocity* vel = locator.get<velocity>(); // thread-safe
///     position* pos = locator.get<position>(); // thread-safe
///
/// Incorrect usage:
///     Thread 1: locator.reg<velocity>();
///     Thread 2: locator.reg<position>(); // ERROR: concurrent registration
///     
///     // Mixing phases
///     Thread 1: locator.get<velocity>();
///     Thread 2: locator.reg<health>(); // ERROR: registration during runtime
///
/// Component types:
///     - Each component type gets unique ID at registration
///     - Maximum types: MAX_COMPONENT_ID
///     - ID assignment is sequential and permanent

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
