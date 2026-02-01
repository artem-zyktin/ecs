#pragma once

#include <cstdint>
#include <limits>

namespace ecs
{

using entity_id = uint32_t;
static constexpr entity_id INVALID_ENTITY_ID = std::numeric_limits<entity_id>::max();
static constexpr entity_id MAX_ENTITY_COUNT = 1024 * 512;

}
