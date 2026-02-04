#pragma once

#include "ecs/component_locator.h"

namespace ecs
{

component_locator::~component_locator()
{
	for (storage_t& item : components_)
	{
		if (item.destroy && item.obj)
		{
			item.destroy(item.obj);
			item.obj = nullptr;
			item.destroy = nullptr;
		}
	}
}

} // namespace ecs
