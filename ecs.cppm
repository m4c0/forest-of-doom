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
  player::compo m_player{};
  sprite::compo m_player_sprites{};

public:
  pog::entity_list &e() noexcept override { return m_e; }

  animation::compo &animations() noexcept override { return m_animations; }
  player::compo &player() noexcept override { return m_player; }
  sprite::compo &player_sprites() noexcept override { return m_player_sprites; }
};
} // namespace ecs
