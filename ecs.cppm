export module ecs;
import animation;
import collision;
import cursor;
import movement;
import pog;
import player;
import sprite;
import tile;
import tilemap;

namespace ecs {
export class ec : public player::compos, public tilemap::compos {
  pog::entity_list m_e{};

  animation::compo m_animations{};

public:
  pog::entity_list &e() noexcept override { return m_e; }

  animation::compo &animations() noexcept override { return m_animations; }
};
} // namespace ecs
