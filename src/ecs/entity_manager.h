#pragma once

#include "ecs/entity.h"

namespace ecs
{

class entity_manager
{
public:
	entity_manager() noexcept = default;
	~entity_manager() noexcept = default;

	entity_manager(const entity_manager&) = delete;
	entity_manager& operator=(const entity_manager&) = delete;

	entity create_entity() noexcept;

private:
	entity_id last_id_ = 0;
};

} // namespace ecs
