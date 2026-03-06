#include "ecs/ecs.h"

#include <iostream>

namespace
{

struct position
{
	float x = 0.0f;
	float y = 0.0f;

	position() noexcept = default;
	position(float px, float py) noexcept : x(px), y(py) {}
};

struct velocity
{
	float vx = 0.0f;
	float vy = 0.0f;

	velocity() noexcept = default;
	velocity(float x, float y) noexcept : vx(x), vy(y) {}
};

struct position_component final : ecs::abstract_component<position>
{
};

struct velocity_component final : ecs::abstract_component<velocity>
{
};

struct movement_system final : ecs::system_base<movement_system, position_component, velocity_component>
{
public:
	void proc(float delta_time, position_component* positions, velocity_component* velocities) override
	{
		if (!positions || !velocities)
		{
			return;
		}

		for (ecs::entity_id id = 1; id < 100; ++id)
		{
			auto* p = positions->get(id);
			auto* v = velocities->get(id);
			if (p && v)
			{
				p->x += v->vx * delta_time;
				p->y += v->vy * delta_time;
			}
		}
	}
};

void basic_gameplay_example()
{
	std::cout << "\n=== Basic gameplay example (tick simulation) ===\n";

	ecs::component_locator world;
	auto* positions = world.add<position_component>();
	auto* velocities = world.add<velocity_component>();

	if (!positions || !velocities)
	{
		std::cout << "Failed to create component storages\n";
		return;
	}

	constexpr ecs::entity_id player = 1;
	constexpr ecs::entity_id enemy = 2;

	positions->emplace(player, 10.0f, 20.0f);
	velocities->emplace(player, 1.5f, 0.0f);

	positions->set(enemy, position{ 50.0f, 40.0f });
	velocities->set(enemy, velocity{ -0.5f, 0.2f });

	movement_system move;
	move.set(positions, velocities);

	constexpr float delta_time = 0.25f;
	constexpr int tick_count = 8;

	for (int tick = 1; tick <= tick_count; ++tick)
	{
		move.run(delta_time);

		std::cout << "Tick " << tick << ":\n";
		for (ecs::entity_id id : { player, enemy })
		{
			auto* p = positions->get(id);
			if (p)
			{
				std::cout << "  Entity " << id << " -> (" << p->x << ", " << p->y << ")\n";
			}
		}
	}
}

void limitations_example()
{
	std::cout << "\n=== Limitations and edge-cases ===\n";

	ecs::component_locator world;
	auto* positions = world.add<position_component>();
	if (!positions)
	{
		std::cout << "Failed to create position storage\n";
		return;
	}

	auto* invalid_insert = positions->set(ecs::INVALID_ENTITY_ID, position{ 1.0f, 1.0f });
	std::cout << "Insert with INVALID_ENTITY_ID -> " << (invalid_insert ? "unexpected success" : "nullptr (expected)") << '\n';

	auto* missing = positions->get(777);
	std::cout << "Get for missing entity -> " << (missing ? "found" : "nullptr (expected)") << '\n';

	std::cout << "Max component types in locator: " << ecs::component_locator::MAX_SIZE << '\n';
	std::cout << "Max entities per component storage: " << position_component::MAX_SIZE << '\n';
}

} // namespace

int main()
{
	basic_gameplay_example();
	limitations_example();

	std::cout << "\nDone.\n";
	return 0;
}
