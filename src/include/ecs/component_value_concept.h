#pragma once

#include <type_traits>

namespace ecs
{

template<typename T>
concept component_value = requires
{
	requires std::is_nothrow_move_constructible_v<T>;
	requires std::is_nothrow_move_assignable_v<T>;
};

} // namespace ecs
