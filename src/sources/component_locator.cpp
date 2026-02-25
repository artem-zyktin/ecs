#include "ecs/component_locator.h"

namespace ecs
{

component_locator::~component_locator() noexcept
{
	for (container_t::size_type idx = 0; idx < container_.size(); ++idx)
	{
		if (container_[idx].p && container_[idx].deleter)
		{
			container_[idx].deleter(container_[idx].p);
		}
	}
}

} // namespace ecs
