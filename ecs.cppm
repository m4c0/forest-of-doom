export module ecs;
import cursor;
import pog;
import sprite;

namespace ecs {
static constexpr const auto max_entities = 1000U;

export struct ec {
  pog::entity_list e{max_entities};
  cursor::compo cursor{};
  sprite::compo sprites{max_entities};
};
} // namespace ecs
