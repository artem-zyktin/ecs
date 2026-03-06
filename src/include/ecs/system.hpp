#include "system.h"
#pragma once

namespace ecs
{

template<typename derived_t, ecs_component ...component_t>
requires is_ecs_system<derived_t, component_t...>
inline void ecs::system_base<derived_t, component_t...>::run(float delta_time)
{
	std::apply([this, delta_time](component_t*... ptrs)
	{
		static_cast<derived_t*>(this)->proc(delta_time, ptrs...);
	},
	components_);
}

template<typename derived_t, ecs_component ...component_t>
requires is_ecs_system<derived_t, component_t...>
inline void system_base<derived_t, component_t...>::set(component_t* ...ptrs) noexcept
{
	components_ = { ptrs... };
}

} // namespace ecs
