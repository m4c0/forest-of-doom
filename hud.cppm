export module hud;
import player;
import pog;
import ranged;
import sprite;
import tile;
import silog;

namespace hud {
export struct anchor {
  float dx;
  float dy;
};
export using battery = ranged(player::c::*);
export class compos : public virtual pog::entity_provider,
                      public virtual player::compos {
  pog::sparse_set<anchor> m_anchors{};
  pog::sparse_set<battery> m_batteries{};
  sprite::compo m_sprites{};

public:
  [[nodiscard]] auto &anchors() noexcept { return m_anchors; }
  [[nodiscard]] auto &batteries() noexcept { return m_batteries; }
  [[nodiscard]] auto &sprites() noexcept { return m_sprites; }
};

auto add_sprite(compos *ec, float x, float y, tile::ui::c t) {
  sprite spr{
      .pos = {0, 0, 1, 1},
      .uv = tile::uv(t),
  };
  auto id = ec->e().alloc();
  ec->anchors().add(id, {x, y});
  ec->sprites().add(id, spr);
  return id;
}

void add_battery(compos *ec, float y, tile::ui::c icon, battery b) {
  add_sprite(ec, 0, y, icon);
  add_sprite(ec, 0.2, y, tile::ui::bat_l);
  auto id = add_sprite(ec, 1.2, y, tile::ui::bat_g_4);
  add_sprite(ec, 2.2, y, tile::ui::bat_r);

  ec->batteries().add(id, b);
}
export void add_entities(compos *ec) {
  add_battery(ec, 3, tile::ui::mind_l, &player::c::happyness);
  add_battery(ec, 2, tile::ui::heart_l, &player::c::health);
  add_battery(ec, 1, tile::ui::food_l, &player::c::satiation);
  add_battery(ec, 0, tile::ui::energy_l, &player::c::energy);
}

export void update_batteries(compos *ec) {
  constexpr const tile::ui::c levels[]{tile::ui::bat_empty, tile::ui::bat_r_1,
                                       tile::ui::bat_y_2, tile::ui::bat_g_3,
                                       tile::ui::bat_g_4};
  auto p = ec->player().get(ec->player().get_id());
  for (auto &[id, attr] : ec->batteries()) {
    unsigned val = static_cast<unsigned>(4.0f * (p.*attr));
    silog::log(silog::debug, "%u", val);

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
