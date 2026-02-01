#include "ecs/entity.h"

namespace ecs
{

entity::entity(entity_id id) noexcept
	: id_(id)
{
}

}
