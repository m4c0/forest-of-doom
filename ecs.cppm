export module ecs;
import anime;
import chunk;
import cursor;
import pog;
import player;
import sprite;
import tilemap;

namespace ecs {
static constexpr const auto max_entities = 1000U;

export class ec : public cursor::compos,
                  public player::compos,
                  public tilemap::compos {
  pog::entity_list m_e{max_entities};

  anime::compo m_animations{16, max_entities};
  chunk::compo m_chunks{max_entities};
  cursor::compo m_cursor{};
  player::compo m_player{};
  sprite::compo m_player_sprites{16, max_entities};
  sprite::compo m_sprites{max_entities};

public:
  pog::entity_list &e() noexcept override { return m_e; }

  anime::compo &animations() noexcept override { return m_animations; }
  chunk::compo &chunks() noexcept override { return m_chunks; }
  cursor::compo &cursor() noexcept override { return m_cursor; }
  player::compo &player() noexcept override { return m_player; }
  sprite::compo &player_sprites() noexcept override { return m_player_sprites; }
  sprite::compo &sprites() noexcept override { return m_sprites; }
};
} // namespace ecs
