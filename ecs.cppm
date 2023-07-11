export module ecs;
import anime;
import collision;
import cursor;
import move;
import movement;
import pog;
import player;
import sprite;
import tile;
import tilemap;

namespace ecs {
export class ec : public movement::compos,
                  public player::compos,
                  public tilemap::compos {
  pog::entity_list m_e{};

  anime::compo m_animations{};
  move::compo m_movements{};
  player::compo m_player{};
  sprite::compo m_player_sprites{};

public:
  pog::entity_list &e() noexcept override { return m_e; }

  anime::compo &animations() noexcept override { return m_animations; }
  move::compo &movements() noexcept override { return m_movements; }
  player::compo &player() noexcept override { return m_player; }
  sprite::compo &player_sprites() noexcept override { return m_player_sprites; }
};
} // namespace ecs
