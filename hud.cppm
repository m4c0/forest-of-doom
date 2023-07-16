export module hud;
import pog;
import sprite;
import tile;

namespace hud {
export struct anchor {
  float dx;
  float dy;
};
export class compos : public virtual pog::entity_provider {
  pog::sparse_set<anchor> m_anchors{};
  sprite::compo m_sprites{};

public:
  [[nodiscard]] auto &anchors() noexcept { return m_anchors; }
  [[nodiscard]] auto &sprites() noexcept { return m_sprites; }
};

void add_sprite(compos *ec, float x, float y, tile::ui::c t) {
  sprite spr{
      .pos = {0, 0, 1, 1},
      .uv = tile::uv(t),
  };
  auto id = ec->e().alloc();
  ec->anchors().add(id, {x, y});
  ec->sprites().add(id, spr);
}

void add_battery(compos *ec, float y, tile::ui::c icon) {
  add_sprite(ec, 0, y, icon);
  add_sprite(ec, 0.2, y, tile::ui::bat_l);
  add_sprite(ec, 1.2, y, tile::ui::bat_g_4);
  add_sprite(ec, 2.2, y, tile::ui::bat_r);
}
export void add_entities(compos *ec) {
  add_battery(ec, 3, tile::ui::mind_l);
  add_battery(ec, 2, tile::ui::heart_l);
  add_battery(ec, 1, tile::ui::food_l);
  add_battery(ec, 0, tile::ui::energy_l);
}

export void update_layout(compos *ec, float gw, float gh) {
  for (auto &[v, id] : ec->anchors()) {
    auto [dx, dy] = v;
    auto spr = ec->sprites().get(id);
    spr.pos.x = -gw + dx;
    spr.pos.y = gh - dy - 1;
    ec->sprites().update(id, spr);
  }
}
} // namespace hud
