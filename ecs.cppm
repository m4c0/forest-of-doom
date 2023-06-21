export module ecs;
import pog;
import sprite;

namespace ecs::c {
export struct marker {};
} // namespace ecs::c
namespace ecs {
static constexpr const auto max_entities = 1000U;

export struct ec {
  pog::entity_list<max_entities> e{};
  pog::sparse_set<sprite> sprites{max_entities};
  pog::singleton<c::marker> cursor{};
};
} // namespace ecs
