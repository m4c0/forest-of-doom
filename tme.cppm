export module main;

import casein;
import collision;
import cursor;
import hai;
import jute;
import missingno;
import pog;
import prefabs;
import qsu;
import silog;
import tile;
import tilemap;
import yoyo_libc;

template <typename C> class palette {
  hai::varray<C> m_tiles;
  unsigned m_index{};

public:
  constexpr palette(auto... n) : m_tiles{sizeof...(n)} {
    (m_tiles.push_back(n), ...);
  }

  [[nodiscard]] constexpr auto &get() noexcept { return m_tiles[m_index]; }

  void operator++() noexcept { m_index = (m_index + 1) % m_tiles.size(); }
  void operator--() noexcept {
    m_index = (m_index + m_tiles.size() - 1) % m_tiles.size();
  }
};
template <typename Tp> palette(Tp, auto...) -> palette<Tp>;

static void fail(const char *msg) {
  silog::log(silog::error, "Error: %s", msg);
  throw 0;
}

namespace camping_set {
namespace t = tile::camping;
static constexpr const auto tname = "tile::camping";
static constexpr const auto tfill = qsu::layers::camping;

static constexpr const auto pals = [] { return palette<palette<t::c>>{}; };
} // namespace camping_set

namespace terrain_set {
namespace t = tile::terrain;
static constexpr const auto tname = "tile::terrain";
static constexpr const auto tfill = qsu::layers::terrain;

static constexpr const auto pals = [] {
  return palette<palette<t::c>>{
      palette{t::island_tl, t::island_t, t::island_tr, t::island_r,
              t::island_br, t::island_b, t::island_bl, t::island_l},
      palette{t::lake_tl, t::island_b, t::lake_tr, t::island_l, t::lake_br,
              t::island_t, t::lake_bl, t::island_r},
      palette{t::grass_tl, t::grass_t, t::grass_tr, t::grass_r, t::grass_br,
              t::grass_b, t::grass_bl, t::grass_l},
      palette{t::dirt_tl, t::grass_b, t::dirt_tr, t::grass_l, t::dirt_br,
              t::grass_t, t::dirt_bl, t::grass_r},
      palette{t::sidewalk_tl, t::sidewalk_t, t::sidewalk_tr, t::sidewalk_r,
              t::sidewalk_br, t::sidewalk_b, t::sidewalk_bl, t::sidewalk_l},
      palette{t::sidewalkr_tl, t::sidewalk_b, t::sidewalkr_tr, t::sidewalk_l,
              t::sidewalkr_br, t::sidewalk_t, t::sidewalkr_bl, t::sidewalk_r},
      palette{t::grass_0, t::grass_1, t::water, t::dirt_0, t::cobble_0,
              t::cobble_1, t::cobble_2, t::cobble_3, t::brick_0, t::brick_1,
              t::brick_2, t::brick_3},
  };
};
} // namespace terrain_set

static constexpr const auto prefab = prefabs::ocean_0;
static constexpr const auto fname = "prefabs-ocean_0.cppm";
static constexpr const auto mname = "ocean_0";

using namespace terrain_set;

struct ec : cursor::compos, t::compos {};

class game {
  ec m_ec{};
  qsu::main *m_q{};
  tilemap::map<t::compos> m_map = prefab;
  tilemap::map<t::compos> m_undo_map = prefab;
  t::c m_brush{};

  palette<palette<t::c>> m_pal = pals();

  void fill_sprites() {
    auto &ec = static_cast<t::compos &>(m_ec);
    t::populate(&m_ec, 0, 0);
    cursor::add_sprite(&m_ec, ec.sprites());
    m_q->fill_sprites(tfill, ec.sprites());
  }

  void update_sprites() {
    auto &ec = static_cast<t::compos &>(m_ec);
    while (ec.tiles().size() > 0) {
      auto [eid, c] = *(ec.tiles().begin());
      t::remove_tile(&m_ec, eid);
    }
    m_map.add_entities(&m_ec, 0, 0);
    fill_sprites();
  }

  void flood_fill(auto x, auto y, t::c old) {
    auto _ = m_map.get(x, y).map([this, x, y, old](auto t) {
      if (t != old)
        return;

      m_map.set(x, y, m_brush);
      flood_fill(x + 1, y, old);
      flood_fill(x - 1, y, old);
      flood_fill(x, y + 1, old);
      flood_fill(x, y - 1, old);
    });
  }

public:
  void setup(qsu::main *q) {
    m_q = q;

    q->set_grid(tilemap::width, tilemap::height);
    q->center_at(tilemap::width / 2, tilemap::height / 2);

    cursor::add_entity(&m_ec);
    update_sprites();
  }

  void set_brush(t::c t) {
    m_brush = t;
    cursor::update_tile(&m_ec, t);
    fill_sprites();
  }

  void next_island_brush() {
    ++m_pal;
    set_brush(m_pal.get().get());
  }
  void prev_island_brush() {
    --m_pal;
    set_brush(m_pal.get().get());
  }
  void next_lake_brush() {
    ++(m_pal.get());
    set_brush(m_pal.get().get());
  }
  void prev_lake_brush() {
    --(m_pal.get());
    set_brush(m_pal.get().get());
  }
  void next_land_brush() {}

  void mouse_moved() {
    auto [x, y] = m_q->mouse_pos();
    cursor::update_pos(&m_ec, x, y);
    fill_sprites();
  }
  void mouse_down() {
    auto [x, y] = m_q->mouse_pos();
    m_undo_map = m_map;
    m_map.set(x, y, m_brush);
    auto [tx, ty, tw, th] = tile::uv(m_brush);
    for (auto dy = 0; dy < th; dy++) {
      for (auto dx = 0; dx < tw; dx++) {
        if (dx == 0 && dy == 0)
          continue;
        m_map.set(x + dx, y + dy, t::blank);
      }
    }
    cursor::update_pos(&m_ec, x, y);
    update_sprites();
  }

  void flood_fill() {
    auto [x, y] = m_q->mouse_pos();
    auto _ = m_map.get(x, y).map([this, x, y](auto t) {
      if (t == m_brush)
        return;
      m_undo_map = m_map;
      flood_fill(x, y, t);
      update_sprites();
    });
  }

  void undo() {
    m_map = m_undo_map;
    update_sprites();
  }

  void dump_map() {
    using namespace jute::literals;

    try {
      yoyo::file_writer out{fname};
      out.writef("export module prefabs:%s;\n", mname).take(fail);
      out.write("import tile;\n"_s).take(fail);
      out.write("import tilemap;\n"_s).take(fail);
      out.write("\n"_s).take(fail);
      out.write("namespace prefabs {\n"_s).take(fail);
      out.writef("export constexpr const tilemap::map %s = [] {\n", mname)
          .take(fail);
      out.writef("  using cs = %s::compos;\n", tname).take(fail);
      out.write("  using c = cs::tile_t;\n"_s).take(fail);
      out.write("  tilemap::map<cs> res{};\n"_s).take(fail);
      for (auto y = 0; y < tilemap::height; y++) {
        for (auto x = 0; x < tilemap::width; x++) {
          m_map.get(x, y)
              .fmap([&](auto t) {
                if (t == 0)
                  return mno::req<void>{};
                return out.writef(
                    "  res.set(%d, %d, static_cast<c>(0x%08x));\n", x, y, t);
              })
              .take(fail);
        }
      }
      out.write("  return res;\n"_s).take(fail);
      out.write("}();\n"_s).take(fail);
      out.write("}\n"_s).take(fail);
    } catch (...) {
    }
    silog::log(silog::info, "Source saved");
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{};

  static constexpr const auto kmap = [] {
    casein::key_map res{};
    res[casein::K_Q] = [](auto) { gg.prev_island_brush(); };
    res[casein::K_W] = [](auto) { gg.next_island_brush(); };
    res[casein::K_A] = [](auto) { gg.prev_lake_brush(); };
    res[casein::K_S] = [](auto) { gg.next_lake_brush(); };
    res[casein::K_E] = [](auto) { gg.next_land_brush(); };
    res[casein::K_D] = [](auto) { gg.flood_fill(); };
    res[casein::K_U] = [](auto) { gg.undo(); };
    res[casein::K_Z] = [](auto) { gg.dump_map(); };
    res[casein::K_SPACE] = [](auto) { gg.set_brush(t::blank); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(&q); };
    res[casein::KEY_DOWN] = [](auto e) { kmap.handle(e); };
    res[casein::MOUSE_MOVE] = [](auto e) { gg.mouse_moved(); };
    res[casein::MOUSE_DOWN] = [](auto e) { gg.mouse_down(); };
    return res;
  }();

  q.process_event(e);
  map.handle(e);
}
