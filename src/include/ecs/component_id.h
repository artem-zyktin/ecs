#pragma once

#include <cstdint>
#include <limits>

namespace ecs
{

using component_id = uint32_t;

inline constexpr component_id INVALID_COMPONENT_ID = std::numeric_limits<component_id>::max();
inline constexpr component_id MAX_COMPONENT_ID = 256;

} // namespace ecs
