export module hud;
import player;
import pog;
import ranged;
import sprite;
import tile;

namespace hud {
struct anchor {
  float dx;
  float dy;
};
export class compos : public virtual pog::entity_provider,
                      public virtual player::compos {
  pog::sparse_set<anchor> m_anchors{};
  pog::sparse_set<pog::marker> m_batteries{};
  sprite::compo m_sprites{};

public:
  [[nodiscard]] auto &anchors() noexcept { return m_anchors; }
  [[nodiscard]] auto &batteries() noexcept { return m_batteries; }
  [[nodiscard]] auto &sprites() noexcept { return m_sprites; }
};

void add_sprite(compos *ec, pog::eid id, float x, float y, tile::ui::c t) {
  sprite spr{
      .pos = {0, 0, 1, 1},
      .uv = tile::uv(t),
  };
  ec->anchors().add(id, {x, y});
  ec->sprites().add(id, spr);
}
auto add_sprite(compos *ec, float x, float y, tile::ui::c t) {
  auto id = ec->e().alloc();
  add_sprite(ec, id, x, y, t);
  return id;
}

void add_battery(compos *ec, float y, tile::ui::c icon, pog::eid gg) {
  add_sprite(ec, 0, y, icon);
  add_sprite(ec, 0.2, y, tile::ui::bat_l);
  add_sprite(ec, gg, 1.2, y, tile::ui::bat_g_4);
  add_sprite(ec, 2.2, y, tile::ui::bat_r);

  ec->batteries().add(gg, {});
}
export void add_entities(compos *ec) {
  add_battery(ec, 3, tile::ui::mind_l, ec->player().happyness);
  // add_battery(ec, 2, tile::ui::heart_l, &player::get_health);
  // add_battery(ec, 1, tile::ui::food_l, &player::get_satiation);
  // add_battery(ec, 0, tile::ui::energy_l, &player::get_energy);
}

export void update_batteries(compos *ec) {
  constexpr const tile::ui::c levels[]{tile::ui::bat_empty, tile::ui::bat_r_1,
                                       tile::ui::bat_y_2, tile::ui::bat_g_3,
                                       tile::ui::bat_g_4};
  auto p = ec->player();
  for (auto &[id, _] : ec->batteries()) {
    auto fv = 4.0f * ec->gauges.get(id).value;
    unsigned iv = static_cast<unsigned>(fv);
    unsigned val = (fv - iv > 0) ? (iv + 1) : iv;

    auto spr = ec->sprites().get(id);
    spr.uv = tile::uv(levels[val]);
    ec->sprites().update(id, spr);
  }
}

export void update_layout(compos *ec, float gw, float gh) {
  for (auto &[id, v] : ec->anchors()) {
    auto [dx, dy] = v;
    auto spr = ec->sprites().get(id);
    spr.pos.x = -gw + dx;
    spr.pos.y = gh - dy - 1;
    ec->sprites().update(id, spr);
  }
}
} // namespace hud
