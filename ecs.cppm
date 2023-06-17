export module ecs;
import pog;

export namespace ecs::c {
struct rect {
  float x;
  float y;
  float w;
  float h;
};
struct sprite {
  rect pos;
  rect uv;
};
} // namespace ecs::c

namespace ecs {
static constexpr const auto max_entities = 100U;

export struct ec {
  pog::entity_list<max_entities> e{};
  pog::sparse_set<c::sprite, max_entities> sprites{};
};
} // namespace ecs
