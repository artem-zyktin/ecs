#pragma once

#include "ecs/abstract_component.h"

#include <type_traits>

namespace ecs
{

template<typename T>
concept ecs_component = requires
{
	typename T::value_type;
	requires std::derived_from<T, abstract_component<typename T::value_type>>;
}
&& std::is_nothrow_default_constructible_v<T>
&& std::is_nothrow_destructible_v<T>;

} // namespace ecs
