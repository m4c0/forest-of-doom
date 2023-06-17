export module ecs;
import pog;

namespace ecs {
static constexpr const auto max_entities = 100U;

export struct ec {
  pog::entity_list<max_entities> e;
};
} // namespace ecs
