#pragma once

#include "ecs/component_concept.h"

#include <tuple>

namespace ecs
{

template<typename T, typename... component_t>
concept is_ecs_system = requires (T system, component_t*... ptrs)
{
	system.proc(ptrs...);
};


template<typename derived_t, ecs_component... component_t>
requires is_ecs_system<derived_t, component_t...>
struct system_base
{
	void run(float delta_time);
	void set(component_t*... ptrs) noexcept;

protected:

	virtual void proc(component_t*... ptr) = 0;

private:
	std::tuple<component_t*...> components_;
};

} // namespace ecs

#include "ecs/system.hpp"
