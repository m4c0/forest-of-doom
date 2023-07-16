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
  add_sprite(ec, -8, y, icon);
  add_sprite(ec, -7.8, y, tile::ui::bat_l);
  add_sprite(ec, -6.8, y, tile::ui::bat_g_4);
  add_sprite(ec, -5.8, y, tile::ui::bat_r);
}
export void add_entities(compos *ec) {
  add_battery(ec, 4, tile::ui::mind_l);
  add_battery(ec, 5, tile::ui::heart_l);
  add_battery(ec, 6, tile::ui::food_l);
  add_battery(ec, 7, tile::ui::energy_l);
}

export void update_layout(compos *ec, float gw, float gh) {
  //
}
} // namespace hud
