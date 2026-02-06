#pragma once

#include "ecs/component_locator.h"

namespace ecs
{

component_id component_locator::last_id_ = INVALID_COMPONENT_ID;

component_locator::~component_locator() noexcept
{
	for (size_type i = 0; i < MAX_COUNT; ++i)
	{
		auto& storage = container_[i];

		if (storage.ptr && storage.deleter)
		{
			storage.deleter(storage.ptr);
			storage.ptr = nullptr;
			storage.deleter = nullptr;
		}
	}
}

} // namespace ecs
