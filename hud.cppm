export module hud;
import player;
import pog;
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

public:
  [[nodiscard]] auto &anchors() noexcept { return m_anchors; }
  [[nodiscard]] auto &batteries() noexcept { return m_batteries; }
};

void add_sprite(compos *ec, pog::eid id, float x, float y, tile::ui::c t) {
  rect pos{0, 0, 1, 1};
  sprite::c spr{
      .uv = tile::uv(t),
      .layer = sprite::layers::ui,
  };
  sprite::add(ec, id, spr, pos);
  ec->anchors().add(id, {x, y});
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
  //add_battery(ec, 3, tile::ui::mind_l, ec->player().happyness);
  //add_battery(ec, 2, tile::ui::heart_l, ec->player().health);
  //add_battery(ec, 1, tile::ui::food_l, ec->player().satiation);
  //add_battery(ec, 0, tile::ui::energy_l, ec->player().energy);
}

// Looks excessive, but it is precise and it allows per-range adjustment (just
// like iPhone batteries now feels like they never leave 100% for some reason).
constexpr unsigned step(float v) {
  if (v == 0)
    return 0;
  if (v < 0.25f)
    return 1;
  if (v < 0.5f)
    return 2;
  if (v < 0.75f)
    return 3;
  return 4;
}
export void update_batteries(compos *ec) {
  constexpr const tile::ui::c levels[]{tile::ui::bat_empty, tile::ui::bat_r_1,
                                       tile::ui::bat_y_2, tile::ui::bat_g_3,
                                       tile::ui::bat_g_4};
  for (auto &[id, _] : ec->batteries()) {
    //unsigned val = step(ec->gauges.get(id).value);
    //sprite::set_uv(ec, id, tile::uv(levels[val]));
  }
}

export void update_layout(compos *ec, float gw, float gh) {
  for (auto &[id, v] : ec->anchors()) {
    auto [dx, dy] = v;
    auto x = -gw + dx;
    auto y = gh - dy - 1;
    sprite::set_pos(ec, id, x, y);
  }
}
} // namespace hud
