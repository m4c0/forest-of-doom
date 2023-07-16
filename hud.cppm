export module hud;
import pog;
import sprite;
import tile;

namespace hud {
export class compos : public virtual pog::entity_provider {
  sprite::compo m_sprites{};

public:
  [[nodiscard]] auto &sprites() noexcept { return m_sprites; }
};

void add_sprite(compos *ec, float x, float y, tile::ui::c t) {
  sprite spr{
      .pos = {x, y, 1, 1},
      .uv = tile::uv(t),
  };
  auto id = ec->e().alloc();
  ec->sprites().add(id, spr);
}

void add_battery(compos *ec, float y, tile::ui::c icon) {
  add_sprite(ec, -4, y, icon);
  add_sprite(ec, -3, y, tile::ui::bat_l);
  add_sprite(ec, -2, y, tile::ui::bat_g_4);
  add_sprite(ec, -1, y, tile::ui::bat_r);
}
export void add_entities(compos *ec) {
  add_battery(ec, 0, tile::ui::mind_l);
  add_battery(ec, 1, tile::ui::heart_l);
  add_battery(ec, 2, tile::ui::food_l);
  add_battery(ec, 3, tile::ui::energy_l);
}
} // namespace hud
