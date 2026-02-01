#include "ecs/entity_id.h"

namespace ecs
{

class entity_manager;

class entity
{
	friend class entity_manager;
public:
	~entity() noexcept = default;

	inline entity_id get_id() const noexcept { return id_; }

protected:
	entity(entity_id id) noexcept;

private:
	entity_id id_;
};

} // namespace ecs
