#pragma once

#include "system.h"

namespace ecs
{

template<typename derived_t, ecs_component ...component_t>
inline void ecs::system_base<derived_t, component_t...>::run(float delta_time)
{
	static_assert(requires(derived_t& system, float dt, component_t*... ptrs)
	{
		system.proc(dt, ptrs...);
	}, "derived system must implement proc(float, component_t*...)");

	std::apply([this, delta_time](component_t*... ptrs)
	{
		static_cast<derived_t*>(this)->proc(delta_time, ptrs...);
	},
	components_);
}

template<typename derived_t, ecs_component ...component_t>
inline void system_base<derived_t, component_t...>::set(component_t* ...ptrs) noexcept
{
	components_ = { ptrs... };
}

} // namespace ecs
