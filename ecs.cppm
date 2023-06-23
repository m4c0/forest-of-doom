export module ecs;
import chunk;
import cursor;
import pog;
import sprite;

namespace ecs {
static constexpr const auto max_entities = 1000U;

export struct ec {
  pog::entity_list e{max_entities};
  cursor::compo cursor{};
  chunk::compo chunks{max_entities};
  sprite::compo sprites{max_entities};
  sprite::compo ui_sprites{max_entities};
};
} // namespace ecs
