export module ecs;
import pog;
import sprite;

namespace ecs {
static constexpr const auto max_entities = 100U;

export struct ec {
  pog::entity_list<max_entities> e{};
  pog::sparse_set<sprite> sprites{max_entities};
};
} // namespace ecs
