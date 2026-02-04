#include "ecs/entity_manager.h"

namespace ecs
{

entity entity_manager::create_entity() noexcept
{
	return entity(last_id_++);
}

} // namespace ecs
